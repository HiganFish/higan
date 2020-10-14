//
// Created by rjd67 on 2020/10/13.
//

#ifndef HIGAN_CODECSERVER_H
#define HIGAN_CODECSERVER_H
#include <higan/TcpServer.h>

class CodecServer
{
public:
	CodecServer(higan::EventLoop* loop, const higan::InetAddress& addr,
			const std::string& server_name);



private:
	higan::TcpServer server_;
};


#endif //HIGAN_CODECSERVER_H
