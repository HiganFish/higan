//
// Created by rjd67 on 2020/7/17.
//

#include "higan/Acceptor.h"

using namespace higan;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& address):
		loop_(loop),
		listening_socket_(address),
		channel_(loop_, "acceptor", listening_socket_.GetFd())
{
	listening_socket_.Bind();
	channel_.EnableReadable();

	channel_.SetReadableCallback(std::bind(&Acceptor::OnNewConnectionInternal, this));
}

Acceptor::~Acceptor()
{
	channel_.DisableAll();
}

void Acceptor::Listen()
{
	listening_socket_.Listen();
}


void Acceptor::OnNewConnectionInternal()
{
	InetAddress address{};
	int client_fd = listening_socket_.Accept(&address);

	if (newconnection_callback_)
	{
		newconnection_callback_(client_fd, address);
	}
	else
	{
		Socket::CloseFd(client_fd);
	}
}

void Acceptor::SetNewConnectionCallback(const Acceptor::NewConnectionCallback& callback)
{
	newconnection_callback_ = callback;
}
