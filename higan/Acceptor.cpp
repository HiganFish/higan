//
// Created by rjd67 on 2020/7/17.
//

#include "higan/Acceptor.h"

using namespace higan;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& address):
		loop_(loop),
		listening_socket_(address),
		channel_(loop_, listening_socket_.GetFd())
{
	listening_socket_.Bind();
	channel_.EnableReadable();
}

Acceptor::~Acceptor()
{

}

void Acceptor::Listen()
{
	listening_socket_.Listen();
}
