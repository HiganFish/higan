//
// Created by rjd67 on 2020/7/21.
//

#include "higan/http/HttpServer.h"

#include "higan/utils/Logger.h"

using namespace higan;

HttpServer::HttpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name):
		server_(loop, addr, server_name),
		file_cache_("HttpCache", 0)
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
	connection->SetContext("HttpContext", HttpContext());
	connection->SetWriteOverCallback(std::bind(&HttpServer::OnMessageSendOver, this, _1));
}

void HttpServer::OnNewMessage(const TcpConnectionPtr& connection, Buffer* buffer)
{
	HttpContext* context =
			std::any_cast<HttpContext>(connection->GetContext("HttpContext"));

	bool parse_ok = context->ParseRequest(buffer);

	if (!parse_ok)
	{
		/**
		 * 解析失败直接简单粗暴返回错误 .... 可以可以 不分配多余的资源
		 */
		connection->Send("HTTP/1.1 400 Bad Request\r\n\r\n");
		connection->DestroyConnection();
	}

	if (context->ParseOver())
	{
		OnHttpRequest(connection, context->GetRequest());
		context->Reset();
	}
}

void HttpServer::OnHttpRequest(const TcpConnectionPtr& connection, HttpRequest& request)
{
	std::string& connection_flag = request["Connection"];

	// 原本的 keep_connection逻辑写起来有点麻烦 换为了跟muduo一样的判断 close
	// 而且只有当连接为close的时候才需要进行特殊操作
	bool close_connection = connection_flag == "close" ||
			(request.GetVersion() == HttpRequest::HTTP_VERSION_10
			&& connection_flag != "Keep-Alive");

	HttpResponse response(&file_cache_ , close_connection);

	if (on_http_request_)
	{
		on_http_request_(connection, request, response);
	}

	Buffer send_buffer;
	response.EncodeToBuffer(&send_buffer);
	connection->Send(&send_buffer);

	if (!response.HasFileToResponse())
	{
		if (close_connection)
		{
			connection->DestroyConnection();
		}
	}
	else
	{
		SendFile(connection, response.GetFilePtr(), close_connection);
	}
}

void HttpServer::SetHttpRequestCallback(const HttpServer::HttpCallback& callback)
{
	on_http_request_ = callback;
}

ssize_t HttpServer::SendFileInternal(const TcpConnectionPtr& connection, const FileContext::FileContextPtr& file_ptr)
{
	ssize_t read_size = -1;
	ssize_t send_size = -1;
	ssize_t sum_send_size = 0;
	Buffer send_buffer;

	ssize_t result = -1;

	while (true)
	{
		read_size = file_ptr->ReadFileToBuffer(&send_buffer);
		if (read_size == 0)
		{
			result = 0;
			break;
		}
		else if (read_size < 0)
		{
			LOG_ERROR << "read file error";
			result = -1;
			break;
		}

		send_size = connection->Send(&send_buffer);
		send_buffer.AddReadIndex(send_size);
		sum_send_size += send_size;

		if (send_size == -1)
		{
			LOG_ERROR << "send file error";
			result = -1;
			break;
		}
		else if (send_size < read_size)
		{
			result =  sum_send_size;
			break;
		}
	}
	return result;
}

void HttpServer::OnMessageSendOver(const TcpConnectionPtr& connection)
{
	/**
	 * 如果存在相关上下文 说明文件未发送完毕 继续发送
	 */
	FileContext::FileContextPtr file_ptr = *std::any_cast<FileContext::FileContextPtr>(
			connection->GetContext("HttpFileContext"));

	ssize_t send_size = SendFileInternal(connection, file_ptr);

	if (send_size > 0)
	{
		connection->SetCallSendOverCallback(true);
	}
	else if (send_size == 0)
	{
		connection->SetCallSendOverCallback(false);
		connection->DeleteContext("HttpFileContext");

		if (file_ptr->CloseConnection())
		{
			connection->DestroyConnection();
		}
	}
	else if (send_size == -1)
	{
		connection->DestroyConnection();
	}
}

void HttpServer::Stop()
{
	server_.Stop();
}

void HttpServer::SendFile(const TcpConnectionPtr& connection, const File::FilePtr& file_ptr_, bool keep_connection)
{
	FileContext::FileContextPtr file_context_ptr = std::make_shared<FileContext>(file_ptr_,
			keep_connection);
	ssize_t send_size = SendFileInternal(connection, file_context_ptr);

	if (send_size > 0)
	{
		connection->SetContext("HttpFileContext", std::any(file_context_ptr));
		connection->SetCallSendOverCallback(true);
	}
	else if (send_size == 0)
	{
		connection->SetCallSendOverCallback(false);
	}
	else if (send_size == -1)
	{
		connection->DestroyConnection();
	}
}

void HttpServer::SetThreadNum(int thread_num)
{
	server_.SetThreadNum(thread_num);
}

void HttpServer::SetMaxFileCacheSize(size_t cache_size)
{
	file_cache_.SetMaxFileSize(cache_size);
}
