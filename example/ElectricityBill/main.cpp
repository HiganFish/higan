//
// Created by rjd67 on 2020/8/5.
//

#include <higan/http/HttpServer.h>
#include <csignal>
#include <higan/base/Logger.h>
#include <higan/base/System.h>
#include "ElectricityBill.h"

std::string web_root = "";

ElectricityBill* g_bill;

void OnHttpRequest(const higan::TcpConnectionPtr& conn, const higan::HttpRequest& request, higan::HttpResponse& response)
{
	if (request.GetMethod() != higan::HttpRequest::HTTP_REQUEST_GET)
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.SetCloseConnection();
		return;
	}

	std::string conn_name = request["X-Real-IP"];
	if (conn_name.empty())
	{
		conn_name = conn->GetConnectionName();
	}

	std::string url = request.GetUrl();
	LOG_INFO << higan::Fmt("connection: %s request %s", conn_name.c_str(),
			url.c_str());

	if (url == "/" || url == "/index.html")
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
		response.SetFileToResponse(web_root + "/index.html");
		return;
	}
	else if (url == "/favicon.ico")
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
		response.SetFileToResponse(web_root + "/favicon.ico");
		return;
	}

	auto result = url.find('?');
	if (result != std::string::npos)
	{
		url = url.substr(0, result);
	}

	if (url.length() > 8)
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.AppendBody("the url: " + request.GetUrl() + " is too long");
		response.SetCloseConnection();
		return;
	}

	std::string path;
	std::string query_result;
	if (g_bill->GetRoomInfoFromUrl(conn_name, url, &path, &query_result))
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);

		response.AppendBody("<!DOCTYPE html>"
							"<html>"
							"<head>"
							"<meta charset=\"utf-8\">"
							"<title>（（（呐）））</title>"
							"</head>"
							"<body>"
	   						"<div style=\"white-space:pre-line\">"
							"<p>网站更新时间 2020-9-6</p>"
	   						"<p>更新内容\n1. 更好的UI ?</p>"
		  					"<hr>");
		response.AppendBody("本次访问查询结果: \n" + query_result + "\n<hr>");
		response.AppendBody("以下是系统自动查询记录\n");
		response.AppendBody("<small>");
		response.SetFileToResponse(path);
	}
	else
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.AppendBody("Illegal url: " + request.GetUrl());
		response.CloseConnection();
	}
}

int main(int argc, char* argv[])
{
	bool daemon = false;
	bool log_to_file = false;
	std::string log_path;

	int opt = -1;
	while ((opt = getopt(argc, argv, "w:l:d")) != -1)
	{
		switch (opt)
		{
			case 'w':
			{
				web_root = optarg;
				break;
			}
			case 'l':
			{
				log_to_file = true;
				log_path = optarg;
				break;
			}
			case 'd':
			{
				daemon = true;
				break;
			}
			default:
				break;
		}
	}

	if (daemon)
	{
		printf("daemon run");
		higan::System::DaemonRun();
	}
	if (log_to_file)
	{
		higan::Logger::SetLogToFile(log_path, "df", false);
	}

	signal(SIGPIPE, SIG_IGN);

	higan::EventLoop loop;

	ElectricityBill bill(&loop, web_root);
	g_bill = &bill;

	higan::InetAddress address{4000};

	higan::HttpServer server{&loop, address, "ElectricityBill"};
	server.SetHttpRequestCallback(OnHttpRequest);

	server.Start();
	loop.Loop();

	return 0;
}