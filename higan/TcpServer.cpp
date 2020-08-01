//
// Created by rjd67 on 2020/7/18.
//

#include "higan/TcpServer.h"
#include "higan/utils/Logger.h"
#include "higan/EventLoop.h"

using namespace higan;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name):
		loop_(loop),
		acceptor_(loop_, addr),
		server_addr_(addr),
		server_name_(server_name)
{
	acceptor_.SetNewConnectionCallback(std::bind(&TcpServer::OnNewConnection,
			this, _1, _2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::Start()
{
	LOG("server: %s, listen on %s", server_name_.c_str(), server_addr_.GetIpPort().c_str());
	acceptor_.Listen();
}

void TcpServer::OnNewConnection(int socket, const InetAddress& address)
{
	std::string connection_name = server_name_ + "-" + address.GetIpPort();

	TcpConnectionPtr connection_ptr(
			new TcpConnection(loop_, connection_name, socket, address));
	connection_ptr->SetMessageCallback(message_callback_);
	connection_ptr->SetErrorCallback(std::bind(&TcpServer::OnConnectionError, this, _1));
	connection_ptr->SetNewConnectionCallback(newconnection_callback_);

	LOG("a new connection %s create", connection_name.c_str());
	connection_map_.insert({socket, connection_ptr});

	connection_ptr->ConnectionEstablished();
}


void TcpServer::OnConnectionError(const TcpConnectionPtr& connection_ptr)
{
	RemoveConnection(connection_ptr);
}

void TcpServer::RemoveConnection(const TcpConnectionPtr& connection_ptr)
{
	std::string connection_name = connection_ptr->GetConnectionName();

	connection_map_.erase(connection_ptr->GetFd());
}


void TcpServer::SetMessageCallback(const MessageCallback & callback)
{
	message_callback_ = callback;
}

void TcpServer::SetMewConnectionCallback(const TcpConnectionCallback& callback)
{
	newconnection_callback_ = callback;
}

bool TcpServer::CloseAllConnection()
{
	bool result = !connection_map_.empty();
	connection_map_.clear();

	return result;
}

