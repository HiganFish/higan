//
// Created by rjd67 on 2020/7/21.
//

#include "higan/http/HttpServer.h"

#include "higan/utils/Logger.h"

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
						   request["Connection"]=="Keep-Alive" : false;

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
		if (SendFile(connection, response.GetFilePath(), keep_connection))
		{
			if (!keep_connection)
			{
				connection->CloseConnection();
			}
		}
	}
	else
	{
		if (!keep_connection)
		{
			connection->CloseConnection();
		}
	}
}

void HttpServer::SetHttpRequestCallback(const HttpServer::OnHttpRequest& callback)
{
	on_http_request_ = callback;
}

ssize_t HttpServer::SendFileInternal(const TcpConnectionPtr& connection, const FileContext::FileContextPtr& file_ptr)
{
	ssize_t read_size = -1;
	ssize_t send_size = -1;
	ssize_t sum_send_size = 0;
	Buffer send_buffer;

	while (true)
	{
		read_size = file_ptr->ReadFileToBuffer(&send_buffer);
		if (read_size == -1)
		{
			LOG_IF(true, "read file error");
			return -1;
		}
		else if(read_size == 0)
		{
			return 0;
		}

		send_size = connection->Send(&send_buffer);
		send_buffer.AddReadIndex(send_size);
		sum_send_size += send_size;

		if (send_size == -1)
		{
			LOG_IF(true, "send file error");
			return -1;
		}
		else if (send_size < read_size)
		{
			return sum_send_size;
		}
	}

	return -1;
}

void HttpServer::OnMessageSendOver(const TcpConnectionPtr& connection)
{
	/**
	 * 如果存在相关上下文 说明文件未发送完毕 继续发送
	 */
	std::any* context_file = nullptr;
	connection->GetContext("HttpFileContext", &context_file);

	FileContext::FileContextPtr file_ptr = *std::any_cast<FileContext::FileContextPtr>(context_file);

	ssize_t send_size = SendFileInternal(connection, file_ptr) > 0;

	if (send_size > 0)
	{
		connection->SetCallSendOverCallback(true);
	}
	else if (send_size == 0)
	{
		connection->SetCallSendOverCallback(false);
		connection->DeleteContext("HttpFileContext");

		if (!file_ptr->IsKeepConnection())
		{
			connection->CloseConnection();
		}
	}
	else
	{
		connection->CloseConnection();
	}
}

bool HttpServer::CloseAllConnection()
{
	return server_.CloseAllConnection();
}

bool HttpServer::SendFile(const TcpConnectionPtr& connection, const std::string& file_url, bool keep_connection)
{
	FileContext::FileContextPtr file_context_ptr = std::make_shared<FileContext>(file_url,
			keep_connection);
	ssize_t send_result = SendFileInternal(connection, file_context_ptr);
	if (send_result > 0)
	{
		connection->SetContext("HttpFileContext", std::any(file_context_ptr));
		connection->SetCallSendOverCallback(true);

		return false;
	}
	else if (send_result == 0)
	{
		connection->SetCallSendOverCallback(false);

		return true;
	}

	return true;
}
