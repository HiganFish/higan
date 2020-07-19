//
// Created by rjd67 on 2020/7/18.
//

#include "higan/TcpServer.h"
#include "higan/utils/Logger.h"

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
	std::string connection_name = server_name_ + "-" + address.GetIpPort();

	TcpConnection::TcpConnectionPtr connection_ptr(
			new TcpConnection(loop_, connection_name, socket, address));
	connection_ptr->SetReadableCallback(std::bind(&TcpServer::OnConnectionReadable, this, std::placeholders::_1));
	connection_ptr->SetWritableCallback(std::bind(&TcpServer::OnConnectionWritable, this, std::placeholders::_1));
	connection_ptr->SetErrorCallback(std::bind(&TcpServer::OnConnectionError, this, std::placeholders::_1));


	LOG("a new connection %s create", connection_name.c_str());
	connection_map_.insert({socket, connection_ptr});

	connection_ptr->ConnectionEstablished();
}

void TcpServer::OnConnectionReadable(const TcpConnection::TcpConnectionPtr& connection_ptr)
{

}

void TcpServer::OnConnectionWritable(const TcpConnection::TcpConnectionPtr& connection_ptr)
{

}

void TcpServer::OnConnectionError(const TcpConnection::TcpConnectionPtr& connection_ptr)
{
	std::string connection_name = connection_ptr->GetConnectionName();
	LOG("connection %s closed", connection_name.c_str());


	connection_map_.erase(connection_ptr->GetFd());
}
