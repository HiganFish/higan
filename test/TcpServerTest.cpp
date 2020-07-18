//
// Created by rjd67 on 2020/7/19.
//

#include <higan/TcpServer.h>

int main()
{

	higan::InetAddress address{1024};

	higan::EventLoop loop;

	higan::TcpServer server(&loop, address, "TcpServerTest");

	server.Start();
	loop.Loop();
}