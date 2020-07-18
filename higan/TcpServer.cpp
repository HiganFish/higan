//
// Created by rjd67 on 2020/7/18.
//

#include "TcpServer.h"

using namespace higan;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name):
		loop_(loop),
		acceptor_(loop_, addr),
		server_name_(server_name)
{
	acceptor_.SetNewConnectionCallback(std::bind(&TcpServer::OnNewConnection,
			this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::Start()
{
	acceptor_.Listen();
}

void TcpServer::OnNewConnection(int socket, const InetAddress& address)
{
	printf("%d--%s\n", socket, address.GetIpPort().c_str());

	Socket::CloseFd(socket);
}
