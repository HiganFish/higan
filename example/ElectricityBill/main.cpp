//
// Created by rjd67 on 2020/8/5.
//

#include <higan/http/HttpServer.h>
#include <csignal>
#include <higan/base/Logger.h>
#include <higan/base/System.h>
#include "ElectricityBill.h"

std::string root = "";

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
		response.SetFileToResponse(root + "/index.html");
		return;
	}
	else if (url == "/favicon.ico")
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
		response.SetFileToResponse(root + "/favicon.ico");
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
		response.AppendBody("重要更新: 现在你手动查询的结果并不会记录下来 而是显示在所有结果最上方 系统只记录每小时的定时查询\n");
		response.AppendBody("now: " + query_result + "\n");
		response.AppendBody("--------------------Results of regular query-----------------\n");
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
	if (argc < 3)
	{
		printf("invalid argument number: %d\n", argc);
		exit(-1);
	}
	root.append(argv[1]);
	const char* log_path = argv[2];

	bool daemon = false;
	bool log_to_file = false;
	if (argc >= 4)
	{
		std::string ex_arg = argv[3];
		for (const char c : ex_arg)
		{
			if (c == 'd')
			{
				daemon = true;
			}
			else if (c == 'f')
			{
				log_to_file = true;
			}
			else
			{
				printf("unknown argument: %s\n", argv[3]);
				exit(0);
			}
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

	ElectricityBill bill(&loop, root);
	g_bill = &bill;

	higan::InetAddress address{4000};

	higan::HttpServer server{&loop, address, "ElectricityBill"};
	server.SetHttpRequestCallback(OnHttpRequest);

	server.Start();
	loop.Loop();

	return 0;
}