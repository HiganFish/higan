//
// Created by rjd67 on 2020/7/25.
//
#include <higan/http/HttpServer.h>
#include <higan/utils/Codec.h>
#include <csignal>
#include <higan/utils/Logger.h>

std::string web_root = "/usr/local/web/blog";

void OnHttpRequest(const higan::TcpConnectionPtr& conn, const higan::HttpRequest& request,
		higan::HttpResponse& response)
{
	LOG_INFO << "request from: " << conn->GetConnectionName();
	LOG_INFO << request.GetMethodString() << " " << request.GetUrl();

//	for (const auto& kv : request.GetHeaderMap())
//	{
//		LOG_INFO << kv.first << ": " << kv.second;
//	}

	response["Server"] = "higan";

	const std::string& url = request.GetUrl();

	if (url == "/hello")
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
		response.AppendBody("hello, world!\n");
	}
	else
	{
		std::string file_path = web_root + request.GetUrl();
		if (response.SetFileToResponse(file_path))
		{
			response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
			LOG_INFO << higan::Fmt("send file: %s success\n", file_path.c_str());
		}
		else
		{
			response.SetStatusCode(higan::HttpResponse::STATUS_404_NOT_FOUND);
			LOG_ERROR << higan::Fmt("send file: %s error\n", file_path.c_str());
		}
	}
}

higan::HttpServer* g_httpserver;

void CloseAllConnection(int signal)
{
	if (signal == SIGINT)
	{
		g_httpserver->Stop();
		exit(signal);
	}
}

int main()
{

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, CloseAllConnection);

	higan::InetAddress address(1022);
	higan::EventLoop loop;

	higan::HttpServer server(&loop, address, "HttpServerTest");
	server.SetThreadNum(3);
	// server.SetMaxFileCacheSize(10 * 1000 * 1000);

	g_httpserver = &server;

	server.SetHttpRequestCallback(OnHttpRequest);

	server.Start();

	loop.Loop();

	return 0;
}