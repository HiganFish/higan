//
// Created by rjd67 on 2020/7/17.
//

#ifndef HIGAN_SOCKET_H
#define HIGAN_SOCKET_H

#include "higan/InetAddress.h"
#include "higan/base/noncopyable.h"

namespace higan
{
class Socket : public noncopyable
{
public:
	explicit Socket(const InetAddress& address);
	Socket(int fd, const InetAddress& address);
	~Socket();

	void Bind();

	void Listen();

	int GetFd() const ;

	/**
	 * 接收新的连接
	 * @param address 存储新连接的地址
	 * @return 新连接的socket描述符 -1代表出错
	 */
	int Accept(InetAddress* address);

	static void CloseFd(int fd);

	void SetReuseAddr();

	void SetReusePort();

	void SetKeepALive();

	ssize_t Send(const char* data, size_t len) const;

	std::string GetAddrPortStr() const;

private:
	int fd_;

	InetAddress address_;
};
}

#endif //HIGAN_SOCKET_H
