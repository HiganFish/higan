//
// Created by rjd67 on 2020/7/19.
//

#ifndef HIGAN_TCPCONNECTION_H
#define HIGAN_TCPCONNECTION_H

#include <string>
#include <memory>
#include <any>
#include <map>

#include "higan/Channel.h"
#include "higan/Socket.h"
#include "higan/Buffer.h"
#include "higan/Callback.h"

namespace higan
{
class EventLoop;
class TcpConnection : public std::enable_shared_from_this<TcpConnection> // 这里如果不加public默认是private.. 滚去了解了解原理再来用吧
																	// 开始确实发现了 _M_weak_this get()显示null 不过却不知道是这个原因
{
public:
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

	void SetNewConnectionCallback(const TcpConnectionCallback& callback);
	void SetMessageCallback(const MessageCallback& callback);
	void SetErrorCallback(const TcpConnectionCallback& callback);
	void SetWriteOverCallback(const TcpConnectionCallback& callback);

	const std::string& GetConnectionName() const;

	int GetFd() const;

	/**
	 * 设置Tcp连接的上下文内容
	 * @param context_key 上下文内容key 用途获取设置的上下文
	 * @param context 需要保存的上下文
	 */
	void SetContext(const std::string& context_key, const std::any& context);
	void DeleteContext(const std::string& context_key);

	/**
	 * 获取此Tcp连接的上下文
	 * @param context_key 上下文key
	 * @param context 上下文结果返回
	 * @return true 存在此key对应的上下文 false不存在对应的上下文
	 */
	bool GetContext(const std::string& context_key, std::any** context);

	ssize_t Send(char* data, size_t len);
	ssize_t Send(Buffer* buffer);

	void CloseConnection();

	bool IsCallSendOverCallback() const;

	void SetCallSendOverCallback(bool callSendOverCallback);

private:
	EventLoop* loop_;
	std::string connection_name_;


	Socket socket_;

	Channel channel_;

	Buffer input_buffer_;
	Buffer output_buffer_;

	bool connecting_;
	bool call_send_over_callback_;

	MessageCallback message_callback_;
	TcpConnectionCallback error_callback_;
	TcpConnectionCallback new_connection_callback_;
	TcpConnectionCallback write_over_callback_;

	std::map<std::string, std::any>  context_map_;

	void OnReadable();

	/**
	 * 如果output_buffer_存在未发送完毕的内容 则继续发送
	 * 发送完毕后取消可写事件回调
	 * 如果 注册了写完回调函数 则调用write_over_callback_
	 */
	void OnWritable();

	void OnError();


};

}
#endif //HIGAN_TCPCONNECTION_H
