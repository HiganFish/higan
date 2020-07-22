//
// Created by rjd67 on 2020/7/21.
//

#include "higan/http/HttpServer.h"
#include "higan/http/HttpContext.h"

using namespace higan;

HttpServer::HttpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name):
		server_(loop, addr, server_name)
{
	server_.SetMewConnectionCallback(std::bind(&HttpServer::OnNewConnection, this, _1));
	server_.SetMessageCallback(std::bind(&HttpServer::OnNewMessage, this, _1, _2));
}

HttpServer::~HttpServer()
{

}

void HttpServer::Start()
{
	server_.Start();
}

void HttpServer::OnNewConnection(const TcpConnectionPtr& connection)
{
	connection->SetContext(HttpContext());
}

void HttpServer::OnNewMessage(const TcpConnectionPtr& connection, Buffer& buffer)
{

}
