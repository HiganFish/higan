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
		channel_(loop_, connection_name, fd),
		connecting_(false)
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
	connecting_ = true;

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
	ssize_t send_size = Send(&output_buffer_);
	if (send_size == -1)
	{
		OnError();
		return;
	}
	else if (static_cast<size_t>(send_size) == output_buffer_.ReadableSize())
	{
		channel_.DisableWritable();
	}

	output_buffer_.AddReadIndex(static_cast<size_t>(send_size));
}

void TcpConnection::OnError()
{
	connecting_ = false;
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

void TcpConnection::SetContext(const std::any& context)
{
	context_ = context;
}

std::any* TcpConnection::GetContext()
{
	return &context_;
}

ssize_t TcpConnection::Send(char* data, size_t len)
{
	if (!connecting_)
	{
		return -1;
	}

	ssize_t send_size = socket_.Send(data, len);

	if (send_size == -1)
	{
		return -1;
	}
	else if (send_size < len)
	{
		output_buffer_.Append(data + send_size, len - static_cast<size_t>(send_size));
		channel_.EnableWritable();
	}

	return send_size;
}

ssize_t TcpConnection::Send(Buffer* buffer)
{
	if (!buffer)
	{
		return -1;
	}
	return Send(buffer->ReadBegin(), buffer->ReadableSize());
}
