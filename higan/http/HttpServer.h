//
// Created by rjd67 on 2020/7/21.
//

#ifndef HIGAN_HTTPSERVER_H
#define HIGAN_HTTPSERVER_H

#include "higan/TcpServer.h"

namespace higan
{
class HttpServer
{
public:
	HttpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name);
	~HttpServer();

	void Start();


private:

	TcpServer server_;

	void OnNewConnection(const TcpConnectionPtr& connection);

	void OnNewMessage(const TcpConnectionPtr& connection, Buffer& buffer);
};
}

#endif //HIGAN_HTTPSERVER_H
