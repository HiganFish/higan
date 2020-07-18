//
// Created by rjd67 on 2020/7/17.
//

#ifndef HIGAN_INETADDRESS_H
#define HIGAN_INETADDRESS_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "higan/utils/copyable.h"

namespace higan
{

class InetAddress : public copyable
{
public:

	InetAddress();
	/**
	 * 提供端口号, 绑定0.0.0.0地址
	 * @param port
	 */
	explicit InetAddress(int port);

	InetAddress(const std::string& ip, int port);

	/**
	 * 填充Ipv4的sockaddr结构体
	 * @param addr
	 */
	void GetSockaddr(sockaddr* addr);

	void SetSockaddr(sockaddr_in* addr);

	/**
	 * @return ip:port
	 */
	std::string GetIpPort() const;
private:

	std::string ip_;
	int port_;
};

}

#endif //HIGAN_INETADDRESS_H
