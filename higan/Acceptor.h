//
// Created by rjd67 on 2020/7/17.
//

#ifndef HIGAN_ACCEPTOR_H
#define HIGAN_ACCEPTOR_H

#include <string>

#include "higan/Socket.h"
#include "higan/Channel.h"
#include "higan/utils/noncopyable.h"

namespace higan
{
class Acceptor : public noncopyable
{
public:
	/**
	 * 提供事件循环和绑定地址
	 * @param loop 事件循环
	 * @param address 地址类
	 */
	Acceptor(EventLoop* loop, const InetAddress& address);
	~Acceptor();

	/**
	 * 调用后Acceptor开始监听连接建立请求
	 */
	void Listen();

private:
	EventLoop* loop_;

	Socket listening_socket_;

	Channel channel_;

	void OnNewConnection();
};
}

#endif //HIGAN_ACCEPTOR_H
