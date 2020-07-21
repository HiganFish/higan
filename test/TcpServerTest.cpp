//
// Created by rjd67 on 2020/7/19.
//

#include <higan/TcpServer.h>

void OnConnection(const higan::TcpConnectionPtr& conn)
{
	printf("---new---\n");
}

void OnMessage(const higan::TcpConnectionPtr& conn, higan::Buffer& buffer)
{
	printf("from %s\n%s\n", conn->GetConnectionName().c_str(), buffer.ReadAllAsString().c_str());
}

int main()
{
	higan::InetAddress address{1024};

	higan::EventLoop loop;

	higan::TcpServer server(&loop, address, "TcpServerTest");
	server.SetMessageCallback(OnMessage);
	server.SetMewConnectionCallback(OnConnection);

	server.Start();
	loop.Loop();
}