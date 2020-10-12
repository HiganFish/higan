//
// Created by rjd67 on 2020/7/19.
//

#include "higan/TcpConnection.h"
#include "higan/EventLoop.h"
#include "higan/base/Logger.h"

using namespace higan;

TcpConnection::TcpConnection(EventLoop* loop, const std::string& connection_name, int fd, const InetAddress& address):
		loop_(loop),
		connection_name_(connection_name),
		socket_(fd, address),
		channel_(loop_, connection_name, fd),
		connecting_(false),
		call_send_over_callback_(false)
{
	channel_.SetReadableCallback(std::bind(&TcpConnection::OnReadable, this));
	channel_.SetWritableCallback(std::bind(&TcpConnection::OnWritable, this));
	channel_.SetErrorCallback(std::bind(&TcpConnection::OnError, this));

}

TcpConnection::~TcpConnection()
{
	channel_.DisableAll();

	if (connecting_)
	{
		LOG_WARN << higan::Fmt("connection: %s closed by destructor", connection_name_.c_str());
	}
}

void TcpConnection::ConnectionEstablished()
{
	if (new_connection_callback_)
	{
		new_connection_callback_(shared_from_this());
	}

	channel_.EnableReadable();
	connecting_ = true;
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
			message_callback_(shared_from_this(), &input_buffer_);
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
		if (call_send_over_callback_)
		{
			if (write_over_callback_)
			{
				write_over_callback_(shared_from_this());
			}
		}
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

void TcpConnection::SetContext(const std::string& context_key, const std::any& context)
{
	context_map_[context_key] = context;
}

ssize_t TcpConnection::Send(const char* data, size_t len)
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
	else if (static_cast<size_t>(send_size) < len)
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

ssize_t TcpConnection::Send(const std::string& data)
{
	return Send(data.c_str(), data.length());
}


void TcpConnection::SetWriteOverCallback(const TcpConnectionCallback& callback)
{
	write_over_callback_ = callback;
}

std::any* TcpConnection::GetContext(const std::string& context_key)
{
	auto result = context_map_.find(context_key);

	if (result == context_map_.end())
	{
		return nullptr;
	}

	return &result->second;
}

void TcpConnection::DeleteContext(const std::string& context_key)
{
	context_map_.erase(context_key);
}

void TcpConnection::DestroyConnection()
{
	if (connecting_)
	{
		connecting_ = false;
		channel_.DisableAll();
		LOG_INFO << higan::Fmt("connection: %s closed", connection_name_.c_str());
	}
}

void TcpConnection::SetCallSendOverCallback(bool callSendOverCallback)
{
	call_send_over_callback_ = callSendOverCallback;
}

EventLoop* TcpConnection::GetEventLoop()
{
	return loop_;
}
