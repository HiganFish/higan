//
// Created by rjd67 on 2020/7/18.
//

#include "higan/TcpServer.h"
#include "higan/base/Logger.h"

using namespace higan;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr, const std::string& server_name):
		loop_(loop),
		thread_pool_(server_name + "Pool", loop),
		server_name_(server_name),
		server_addr_(addr),
		acceptor_(loop_, addr),
		connection_map_()
{
	acceptor_.SetNewConnectionCallback(std::bind(&TcpServer::OnNewConnection,
			this, _1, _2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::Start()
{
	thread_pool_.Start();

	LOG_INFO << higan::Fmt("server: %s, listen on %s", server_name_.c_str(), server_addr_.GetIpPort().c_str());
	acceptor_.Listen();
}

void TcpServer::OnNewConnection(int socket, const InetAddress& address)
{
	std::string connection_name = server_name_ + "-" + address.GetIpPort();

	EventLoop* loop = thread_pool_.GetNextEventLoop();

	TcpConnectionPtr connection_ptr(
			new TcpConnection(loop, connection_name, socket, address));

	connection_ptr->SetMessageCallback(message_callback_);
	connection_ptr->SetErrorCallback(std::bind(&TcpServer::OnConnectionError, this, _1));
	connection_ptr->SetNewConnectionCallback(newconnection_callback_);

	LOG_INFO << higan::Fmt("a new connection: %s create", connection_name.c_str());
	connection_map_.insert({socket, connection_ptr});

	/**
	 * 在新连接所属的loop中执行
	 * 这里因为是主线程运行, 所以ConnectionEstablished会被放入从线程的队列中
	 * 等待从线程执行
	 */
	loop->RunInLoop(std::bind(&TcpConnection::ConnectionEstablished, connection_ptr));
}


void TcpServer::OnConnectionError(const TcpConnectionPtr& connection_ptr)
{
	RemoveConnection(connection_ptr);
}

void TcpServer::RemoveConnection(const TcpConnectionPtr& connection_ptr)
{
	loop_->RunInLoop(std::bind(&TcpServer::RemoveConnectionInLoop, this, connection_ptr));
}

void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& connection_ptr)
{
	/**
	 * 多线程情况下
	 * muduo的设计思想就是一个线程对应一个事件循环EventLoop
	 * 因为 RemoveConnection 会被多个线程执行, 所以需要加锁.
	 * 但是 TcpServer的loop_从属于主线程
	 * 依托多线程的变量共享, 本该在从线程执行的函数 可以注册到TcpServer的loop_中
	 * 而TcpServer的loop_ 正好对应于主线程
	 * 从而实现了在主线程统一处理 所有从线程RunInLoop的函数
	 */

	std::string connection_name = connection_ptr->GetConnectionName();
	LOG_INFO << higan::Fmt("erase a connection %s", connection_name.c_str());

	connection_map_.erase(connection_ptr->GetFd());

	connection_ptr->GetEventLoop()->RunInLoop(
			std::bind(&TcpConnection::DestroyConnection, connection_ptr)
			);
}

void TcpServer::SetMessageCallback(const MessageCallback & callback)
{
	message_callback_ = callback;
}

void TcpServer::SetMewConnectionCallback(const TcpConnectionCallback& callback)
{
	newconnection_callback_ = callback;
}

void TcpServer::Stop()
{
	connection_map_.clear();
	thread_pool_.Stop();
}

void TcpServer::SetThreadNum(int thread_num)
{
	thread_pool_.SetThreadNum(thread_num);
}
