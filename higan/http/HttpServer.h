//
// Created by rjd67 on 2020/7/21.
//

#ifndef HIGAN_HTTPSERVER_H
#define HIGAN_HTTPSERVER_H

#include "higan/TcpServer.h"
#include "higan/http/HttpContext.h"
#include "higan/http/HttpResponse.h"
#include "higan/utils/FileCache.h"

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

	FileCache file_cache_;

	void OnNewConnection(const TcpConnectionPtr& connection);

	void OnNewMessage(const TcpConnectionPtr& connection, Buffer& buffer);

	void OnMessageSendOver(const TcpConnectionPtr& connection);

	void ParseOver(const TcpConnectionPtr& connection, HttpRequest& request);

	void SendFile(const TcpConnectionPtr& connection, const FileCache::FilePtr& file_ptr);
};
}

#endif //HIGAN_HTTPSERVER_H
