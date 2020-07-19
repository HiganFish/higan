//
// Created by rjd67 on 2020/7/19.
//

#ifndef HIGAN_TCPCONNECTION_H
#define HIGAN_TCPCONNECTION_H

#include <string>
#include <memory>

#include "higan/Channel.h"
#include "higan/Socket.h"

namespace higan
{
class EventLoop;
class TcpConnection : public std::enable_shared_from_this<TcpConnection> // 这里如果不加public默认是private.. 滚去了解了解原理再来用吧
																	// 开始确实发现了 _M_weak_this get()显示null 不过却不知道是这个原因
{
public:
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void(const TcpConnectionPtr&)> TcpConnectionCallback;

	/**
	 * Tcpconnection 负责维护连接 读写缓冲区
	 * @param loop 事件循环
	 * @param connection_name 连接名称
	 * @param fd 连接文件描述符
	 * @param address 连接客户端地址
	 */
	TcpConnection(EventLoop* loop, const std::string& connection_name, int fd, const InetAddress& address);

	~TcpConnection();

	void ConnectionEstablished();

	void SetReadableCallback(const TcpConnectionCallback& callback);
	void SetWritableCallback(const TcpConnectionCallback& callback);
	void SetErrorCallback(const TcpConnectionCallback& callback);

	const std::string& GetConnectionName() const;

	int GetFd() const;
private:
	EventLoop* loop_;
	std::string connection_name_;

	Socket socket_;

	Channel channel_;

	TcpConnectionCallback readable_callback_;
	TcpConnectionCallback writable_callback_;
	TcpConnectionCallback error_callback_;

	void OnReadable();

	void OnWritable();

	void OnError();


};

}
#endif //HIGAN_TCPCONNECTION_H