//
// Created by rjd67 on 2020/7/25.
//
#include <iostream>

#include <higan/http/HttpServer.h>
#include <higan/EventLoop.h>

std::string web_root = "/usr/local/web/blog";

void OnHttpRequest(const higan::TcpConnectionPtr& conn, const higan::HttpRequest& request,
		higan::HttpResponse& response)
{
	std::cout << "request from: " << conn->GetConnectionName() << std::endl;
	std::cout << request.GetMethodString() << " " << request.GetUrl() << std::endl;

	std::string file_path = web_root + request.GetUrl();

	struct stat file_status;

	if (stat(file_path.c_str(), &file_status) == -1)
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_404_NOT_FOUND);
		response.AppendBody("file don't exist");
		return;
	}

	if (S_ISDIR(file_status.st_mode))
	{
		file_path += "/index.html";

		if (stat(file_path.c_str(), &file_status) == -1)
		{
			response.SetStatusCode(higan::HttpResponse::STATUS_404_NOT_FOUND);
			response.AppendBody("file don't exist");
			return;
		}
	}

	response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
	response.SetFileToResponse(file_path);
}

int main()
{

	higan::InetAddress address(1020);
	higan::EventLoop loop;

	higan::HttpServer server(&loop, address, "HttpServerTest");
	server.SetHttpRequestCallback(OnHttpRequest);

	server.Start();

	loop.Loop();
}