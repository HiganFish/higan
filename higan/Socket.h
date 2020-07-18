//
// Created by rjd67 on 2020/7/17.
//

#ifndef HIGAN_SOCKET_H
#define HIGAN_SOCKET_H

#include "higan/InetAddress.h"

namespace higan
{
class Socket
{
public:
	explicit Socket(const InetAddress& address);
	Socket(int fd, const InetAddress& address);
	~Socket();

	void Bind();

	void Listen();

	int GetFd() const ;

private:
	int fd_;

	InetAddress address_;
};
}

#endif //HIGAN_SOCKET_H
