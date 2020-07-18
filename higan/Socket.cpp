//
// Created by rjd67 on 2020/7/17.
//

#include <sys/socket.h>
#include <unistd.h>

#include "higan/utils/Logger.h"
#include "higan/Socket.h"

using namespace higan;

Socket::Socket(const InetAddress& address):
		fd_(socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0)),
		address_(address)
{

}

Socket::Socket(int fd, const InetAddress& address):
		fd_(fd),
		address_(address)
{

}

Socket::~Socket()
{
	close(fd_);
}

void Socket::Bind()
{
	struct sockaddr addr{};
	address_.GetSockaddr(&addr);
	int result = bind(fd_, &addr, static_cast<socklen_t>(sizeof addr));

	LOG_IF(result == -1, "bind on %s error", address_.GetIpPort().c_str());
}

void Socket::Listen()
{
	listen(fd_, 5);
}

int Socket::GetFd() const
{
	return fd_;
}
