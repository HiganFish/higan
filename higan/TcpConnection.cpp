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

	if (new_connection_callback_)
	{
		new_connection_callback_(shared_from_this());
	}
}

void TcpConnection::OnReadable()
{
	ssize_t len = input_buffer_.ReadFromFd(socket_.GetFd());

	if (len == 0)
	{
		OnError();
	}
	else
	{
		if (message_callback_)
		{
			message_callback_(shared_from_this(), input_buffer_);
		}
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

void TcpConnection::SetMessageCallback(const MessageCallback& callback)
{
	message_callback_ = callback;
}

void TcpConnection::SetErrorCallback(const TcpConnectionCallback& callback)
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

void TcpConnection::SetNewConnectionCallback(const TcpConnectionCallback& callback)
{
	new_connection_callback_ = callback;
}
