//
// Created by rjd67 on 2020/7/21.
//

#ifndef HIGAN_HTTPSERVER_H
#define HIGAN_HTTPSERVER_H

#include "higan/TcpServer.h"
#include "higan/http/HttpContext.h"
#include "higan/http/HttpResponse.h"

namespace higan
{
class HttpServer
{
public:
	typedef  std::function<void(const TcpConnectionPtr& connection, const HttpRequest& request,
			HttpResponse& response)> OnHttpRequest;


	HttpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name);
	~HttpServer();

	void Start();

	void SetHttpRequestCallback(const OnHttpRequest& callback);

private:

	TcpServer server_;

	OnHttpRequest on_http_request_;

	void OnNewConnection(const TcpConnectionPtr& connection);

	void OnNewMessage(const TcpConnectionPtr& connection, Buffer& buffer);

	void ParseOver(const TcpConnectionPtr& connection, HttpRequest& request);
};
}

#endif //HIGAN_HTTPSERVER_H
