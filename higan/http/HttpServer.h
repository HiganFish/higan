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

	void Stop();

	void SetThreadNum(int thread_num);

	/**
	 * 设置单文件的缓存判断上限
	 * @param cache_size 大小小于此值的文件将被缓存 单位字节
	 */
	void SetMaxFileCacheSize(size_t cache_size);
private:

	TcpServer server_;

	FileCache file_cache_;

	HttpCallback on_http_request_;

	void OnNewConnection(const TcpConnectionPtr& connection);

	void OnNewMessage(const TcpConnectionPtr& connection, Buffer* buffer);

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
