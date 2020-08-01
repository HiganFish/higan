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
	/**
	 * 按照自己的编程规范 response应该声明为指针 可是这样会导致重载的 [] 运算符使用不便 所以选择了引用
	 */
	typedef  std::function<void(const TcpConnectionPtr& connection, const HttpRequest& request,
			HttpResponse& response)> HttpCallback;


	HttpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name);
	~HttpServer();

	void Start();

	void SetHttpRequestCallback(const HttpCallback& callback);

	bool CloseAllConnection();

private:

	TcpServer server_;

	HttpCallback on_http_request_;

	void OnNewConnection(const TcpConnectionPtr& connection);

	void OnNewMessage(const TcpConnectionPtr& connection, Buffer& buffer);

	void OnMessageSendOver(const TcpConnectionPtr& connection);

	void OnHttpRequest(const TcpConnectionPtr& connection, HttpRequest& request);

	/**
	 * 发送文件到指定连接
	 * @param connection 发送到的连接
	 * @param file_ptr_ 待发送文件路径
	 * @param keep_connection 保持连接
	 * @return
	 */
	void SendFile(const TcpConnectionPtr& connection, const File::FilePtr& file_ptr_, bool keep_connection);

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
