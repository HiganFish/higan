//
// Created by rjd67 on 2020/7/19.
//

#include "higan/TcpConnection.h"
#include "higan/EventLoop.h"
#include "higan/utils/Logger.h"

using namespace higan;

TcpConnection::TcpConnection(EventLoop* loop, const std::string& connection_name, int fd, const InetAddress& address):
		loop_(loop),
		connection_name_(connection_name),
		socket_(fd, address),
		channel_(loop_, connection_name, fd)
{
	channel_.SetReadableCallback(std::bind(&TcpConnection::OnReadable, this));
	channel_.SetWritableCallback(std::bind(&TcpConnection::OnWritable, this));
	channel_.SetErrorCallback(std::bind(&TcpConnection::OnError, this));

}

TcpConnection::~TcpConnection()
{
	channel_.DisableAll();
}

void TcpConnection::ConnectionEstablished()
{
	channel_.EnableReadable();
}

void TcpConnection::OnReadable()
{
	char read_buffer[100]{};
	ssize_t len = recv(socket_.GetFd(), read_buffer, sizeof read_buffer, 0);
	if (len == 0)
	{
		OnError();
	}
	else
	{
		LOG("recv from: %s\n%s\n", connection_name_.c_str(), read_buffer);
	}
}

void TcpConnection::OnWritable()
{

}

void TcpConnection::OnError()
{
	channel_.DisableAll();
	if (error_callback_)
	{
		error_callback_(shared_from_this());
	}
}

void TcpConnection::SetReadableCallback(const TcpConnection::TcpConnectionCallback& callback)
{
	readable_callback_ = callback;
}

void TcpConnection::SetWritableCallback(const TcpConnection::TcpConnectionCallback& callback)
{
	writable_callback_ = callback;
}

void TcpConnection::SetErrorCallback(const TcpConnection::TcpConnectionCallback& callback)
{
	error_callback_ = callback;
}

const std::string& TcpConnection::GetConnectionName() const
{
	return connection_name_;
}

int TcpConnection::GetFd() const
{
	return socket_.GetFd();
}
