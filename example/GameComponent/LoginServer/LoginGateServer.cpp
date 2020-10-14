//
// Created by rjd67 on 2020/10/13.
//

#include <higan/InetAddress.cpp>
#include <higan/TcpServer.h>
#include <higan/base/Logger.h>
#include <higan/base/Singleton.h>

#include "UserManager.h"

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

	const std::string& auth_key = package.GetAuthenticationKey();
	std::string username = auth_key.substr(0, auth_key.find('#'));
	std::string password = auth_key.substr(auth_key.find('#') + 1,
			auth_key.length());

	ELoginResult result = higan::Singleton<UserManager>::Instance().Login(username, password);

	uint32_t user_id = random() % UINT32_MAX;

	LOG_INFO << connection_ptr->GetConnectionName() << " username: " << username <<
			 " ,password: " << password << " ,user_id: " << std::to_string(user_id) <<
			 " ,result: " << std::to_string(result);

	LoginResultPackage login_result_package(result, user_id);

	uint8_t send_buffer[512];
	login_result_package.SerializeToBuffer(send_buffer, sizeof send_buffer);
	uint32_t msg_len = login_result_package.GetPackageLen();

	for (size_t i = 0; i < msg_len; ++i)
	{
		printf("%02X ", send_buffer[i]);
	}
	printf("\n");

	connection_ptr->Send((const char*)send_buffer, msg_len);
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