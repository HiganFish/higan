//
// Created by rjd67 on 2020/7/18.
//

#include <higan/Acceptor.h>
#include <higan/EventLoop.h>

int main()
{
	higan::InetAddress address(1024);

	higan::EventLoop loop;

	higan::Acceptor acceptor(&loop, address);
	acceptor.Listen();

	loop.Loop();
}