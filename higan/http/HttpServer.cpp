//
// Created by rjd67 on 2020/7/21.
//

#include "higan/http/HttpServer.h"

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
	const char* data = buffer.ReadBegin();
	size_t size = buffer.ReadableSize();

	HttpContext* context = std::any_cast<HttpContext>(connection->GetContext());

	ssize_t parsed_size = context->ParseRequest(data, size);

	if (parsed_size == -1)
	{

	}
	else
	{
		buffer.AddReadIndex(static_cast<size_t>(parsed_size));
	}

	if (!context->ParseOver())
	{
		return;
	}

	ParseOver(connection, context->GetRequest());
}

void HttpServer::ParseOver(const TcpConnectionPtr& connection, HttpRequest& request)
{
	bool keep_connection = request.GetVersion()==HttpRequest::HTTP_VERSION_11 ?
						   request["Connection"]=="keep-alive" : false;

	HttpResponse response(keep_connection);

	if (on_http_request_)
	{
		on_http_request_(connection, request, response);
	}

}

void HttpServer::SetHttpRequestCallback(const HttpServer::OnHttpRequest& callback)
{
	on_http_request_ = callback;
}
