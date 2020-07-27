//
// Created by rjd67 on 2020/7/21.
//

#include "higan/http/HttpServer.h"

#include "higan/utils/Logger.h"

using namespace higan;

HttpServer::HttpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name):
		server_(loop, addr, server_name),
		file_cache_()
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
	connection->SetContext("HttpContext", std::any(HttpContext()));
	connection->SetWriteOverCallback(std::bind(&HttpServer::OnMessageSendOver, this, _1));
}

void HttpServer::OnNewMessage(const TcpConnectionPtr& connection, Buffer& buffer)
{
	const char* data = buffer.ReadBegin();
	size_t size = buffer.ReadableSize();

	std::any* context_any = nullptr;
	connection->GetContext("HttpContext", &context_any);
	HttpContext* context = std::any_cast<HttpContext>(context_any);

	ssize_t parsed_size = context->ParseRequest(data, size);

	if (parsed_size == -1)
	{
		HttpResponse response(false);
		response.SetStatusCode(HttpResponse::STATUS_400_BAD_REQUEST);
		Buffer send_buffer;
		response.EncodeToBuffer(&send_buffer);
		connection->Send(&send_buffer);
		return;
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
	context->Reset();
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

	Buffer send_buffer;
	response.EncodeToBuffer(&send_buffer);
	connection->Send(&send_buffer);

	if (response.HasFileToResponse())
	{
		FileCache::FilePtr file_ptr = file_cache_.GetFile(response.GetFilePath());
		SendFile(connection, file_ptr);
	}


	/**
	 * 如果有文件需要发送
	 * 注册文件上下文
	 * 持续调用Send函数发送直到
	 *  - 无法发送但文件未发送完, TcpConnection注册了写完回调 待发送完已经Send的内容后 调用写完回调继续写
	 *  - 文件发送完 xxxxx
	 */
}

void HttpServer::SetHttpRequestCallback(const HttpServer::OnHttpRequest& callback)
{
	on_http_request_ = callback;
}

void HttpServer::SendFile(const TcpConnectionPtr& connection, const FileCache::FilePtr& file_ptr)
{
	ssize_t read_size = -1;
	ssize_t send_size = -1;
	Buffer send_buffer;

	while (true)
	{
		read_size = file_ptr->ReadFileToBuffer(&send_buffer);
		if (read_size == -1)
		{
			LOG_IF(true, "read file error");
			return;
		}
		else if(read_size == 0)
		{
			break;
		}

		send_size = connection->Send(&send_buffer);
		if (send_size == -1)
		{
			LOG_IF(true, "send file error");
			return;
		}
		else if (send_size < read_size)
		{
			connection->SetContext("HttpFile", std::any(file_ptr));
			break;
		}
	}

	connection->DeleteContext("HttpFile");
}

void HttpServer::OnMessageSendOver(const TcpConnectionPtr& connection)
{
	/**
	 * 如果存在相关上下文 说明文件未发送完毕 继续发送
	 */
	std::any* context_file = nullptr;
	bool has_file = connection->GetContext("HttpFile", &context_file);
	if (!has_file)
	{
		return;
	}

	FileCache::FilePtr* file_ptr = std::any_cast<FileCache::FilePtr>(context_file);
	SendFile(connection, *file_ptr);
}
