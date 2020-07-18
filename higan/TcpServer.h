//
// Created by rjd67 on 2020/7/18.
//

#ifndef HIGAN_TCPSERVER_H
#define HIGAN_TCPSERVER_H

#include <string>

#include "higan/utils/noncopyable.h"
#include "higan/InetAddress.h"
#include "higan/Acceptor.h"
#include "higan/EventLoop.h"

namespace higan
{
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
private:

	EventLoop* loop_;
	std::string server_name_;

	Acceptor acceptor_;

	/**
	 * 供Acceptor在 新连接建立后调用
	 * @param socket 文件描述符
	 * @param address 地址
	 */
	void OnNewConnection(int socket, const InetAddress& address);
};
}

#endif //HIGAN_TCPSERVER_H