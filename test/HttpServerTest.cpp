//
// Created by rjd67 on 2020/7/25.
//
#include <iostream>

#include <higan/http/HttpServer.h>
#include <higan/EventLoop.h>

void OnHttpRequest(const higan::TcpConnectionPtr& conn, const higan::HttpRequest& request,
		higan::HttpResponse& response)
{
	std::cout << "request from: " << conn->GetConnectionName() << std::endl;
	std::cout << request.GetMethodString() << " " << request.GetUrl() << std::endl;

	for (const auto& kv : request.GetHeaderMap())
	{
		std::cout << kv.first << ": " << kv.second << std::endl;
	}
	std::cout << std::endl;

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