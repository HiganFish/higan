//
// Created by rjd67 on 2020/10/13.
//

#include <higan/InetAddress.cpp>
#include <higan/TcpServer.h>
#include <higan/base/Logger.h>

#include "LoginNetworkPackage.h"

void OnMessage(const TcpConnectionPtr& connection_ptr, Buffer* buffer)
{
	const char* data = buffer->ReadBegin();
	size_t data_len = buffer->ReadableSize();

	LoginNetworkPackage package;
	ssize_t parsed_len = package.DeSerializeFromBuffer((uint8_t*)data, data_len);

	if (parsed_len == 0)
	{
		LOG_DEBUG << connection_ptr->GetConnectionName() << " data is too little";
		return;
	}
	else if (parsed_len < 0)
	{
		LOG_WARN << connection_ptr->GetConnectionName() << " send error data";
		return;
	}
	buffer->AddReadIndex(static_cast<size_t>(parsed_len));

	LOG_INFO << connection_ptr->GetConnectionName() << " authentication key " << package.GetAuthenticationKey();

}

int main()
{
	higan::InetAddress server_address{8700};


	higan::EventLoop loop;
	higan::TcpServer server{&loop, server_address, "LoginGateServer"};
	server.SetMessageCallback(OnMessage);

	server.Start();
	loop.Loop();
}