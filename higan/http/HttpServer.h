//
// Created by rjd67 on 2020/7/21.
//

#ifndef HIGAN_HTTPSERVER_H
#define HIGAN_HTTPSERVER_H

#include "higan/TcpServer.h"
#include "higan/http/HttpContext.h"
#include "higan/http/HttpResponse.h"
#include "higan/http/FileContext.h"

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

	bool CloseAllConnection();

private:

	TcpServer server_;

	OnHttpRequest on_http_request_;

	void OnNewConnection(const TcpConnectionPtr& connection);

	void OnNewMessage(const TcpConnectionPtr& connection, Buffer& buffer);

	void OnMessageSendOver(const TcpConnectionPtr& connection);

	void ParseOver(const TcpConnectionPtr& connection, HttpRequest& request);

	/**
	 * 发送文件到指定连接
	 * @param connection 发送到的连接
	 * @param file_url 待发送文件路径
	 * @param keep_connection 保持连接
	 * @return true 发送完毕或出错 false 未发送完毕
	 */
	bool SendFile(const TcpConnectionPtr& connection, const std::string& file_url, bool keep_connection);

	/**
	 * 发送文件到指定连接
	 * @param connection 发送到的连接
	 * @param file_ptr 待发送文件上下文指针
	 * @return -1 发送出错 0 发送完毕 大于0 发送的字节数
	 */
	ssize_t SendFileInternal(const TcpConnectionPtr& connection, const FileContext::FileContextPtr & file_ptr);
};
}

#endif //HIGAN_HTTPSERVER_H
