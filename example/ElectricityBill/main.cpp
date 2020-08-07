//
// Created by rjd67 on 2020/8/5.
//

#include <higan/http/HttpServer.h>
#include <signal.h>
#include <higan/utils/Logger.h>
#include "ElectricityBill.h"

std::string root = "/usr/local/web/df-5100/";

ElectricityBill* g_bill;

void OnHttpRequest(const higan::TcpConnectionPtr& conn, const higan::HttpRequest& request, higan::HttpResponse& response)
{
	if (request.GetMethod() != higan::HttpRequest::HTTP_REQUEST_GET)
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.SetCloseConnection();
		return;
	}

	std::string url = request.GetUrl();
	LOG_INFO << higan::Fmt("connection: %s request %s", conn->GetConnectionName().c_str(),
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
	if (g_bill->GetRoomFilePath(conn->GetConnectionName(), url, &path))
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
		response.SetFileToResponse(path);
	}
	else
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.AppendBody("Illegal url: " + request.GetUrl());
		response.CloseConnection();
	}
}

int main()
{
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