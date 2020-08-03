//
// Created by rjd67 on 2020/7/18.
//

#ifndef HIGAN_TCPSERVER_H
#define HIGAN_TCPSERVER_H

#include <string>
#include <map>

#include "higan/utils/noncopyable.h"
#include "higan/Acceptor.h"
#include "higan/TcpConnection.h"
#include "higan/EventLoopThreadPool.h"

namespace higan
{
class EventLoop;

class TcpServer : public noncopyable
{
public:
	/**
	 * 创建Tcp服务器
	 * @param loop 从属事件循环
	 * @param addr 坚挺地址
	 * @param server_name 服务器名称
	 */
	TcpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name);
	~TcpServer();

	void Start();

	void SetThreadNum(int thread_num);

	void SetMessageCallback(const MessageCallback& callback);
	void SetMewConnectionCallback(const TcpConnectionCallback& callback);

	void Stop();

private:

	typedef std::map<int, TcpConnectionPtr> TcpConnectionMap;

	EventLoop* loop_;
	EventLoopThreadPool thread_pool_;
	std::string server_name_;

	Acceptor acceptor_;
	InetAddress server_addr_;

	TcpConnectionMap connection_map_;

	MessageCallback message_callback_;
	TcpConnectionCallback newconnection_callback_;


	/**
	 * 供Acceptor在 新连接建立后调用
	 * @param socket 文件描述符
	 * @param address 地址
	 */
	void OnNewConnection(int socket, const InetAddress& address);

	void RemoveConnection(const TcpConnectionPtr& connection_ptr);

	void OnConnectionError(const TcpConnectionPtr& connection_ptr);

};
}

#endif //HIGAN_TCPSERVER_H
