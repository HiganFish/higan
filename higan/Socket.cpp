//
// Created by rjd67 on 2020/7/17.
//

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include "higan/base/Logger.h"
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
	SetKeepALive();
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

	if (result == -1)
	{
		LOG_FATAL << higan::Fmt("bind on %s error", address_.GetIpPort().c_str());
	}
}

void Socket::SetReuseAddr()
{
	int on = 1;
	int result = setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, static_cast<void*>(&on),
			static_cast<socklen_t>(sizeof on));

	if (result == -1)
	{
		LOG_FATAL << "set socket reuse addr error";
	}
}

void Socket::SetReusePort()
{
	int on = 1;
	int result = setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, static_cast<void*>(&on),
			static_cast<socklen_t>(sizeof on));

	if (result == -1)
	{
		LOG_FATAL <<  "set socket reuse port error";
	}
}

void Socket::SetKeepALive()
{
	int on = 1;
	int result = setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, static_cast<void*>(&on),
			static_cast<socklen_t>(sizeof on));

	if (result == -1)
	{
		LOG_FATAL << "set socket reuse port error";
	}
}

void Socket::Listen()
{
	listen(fd_, 5);
}

int Socket::GetFd() const
{
	return fd_;
}


int Socket::Accept(InetAddress* address)
{
	if (address == nullptr)
	{
		return -1;
	}

	struct sockaddr_in client_addr{};
	socklen_t sockaddr_len = static_cast<socklen_t>(sizeof client_addr);

	int result_fd = accept4(fd_, reinterpret_cast<sockaddr*>(&client_addr),
			&sockaddr_len, SOCK_CLOEXEC);

	address->SetSockaddr(&client_addr);

	return result_fd;
}

void Socket::CloseFd(int fd)
{
	close(fd);
}

ssize_t Socket::Send(const char* data, size_t len) const
{
	if (data == nullptr)
	{
		return -1;
	}
	return send(fd_, data, len, 0);
}


std::string Socket::GetAddrPortStr() const
{
	return address_.GetIpPort();
}
