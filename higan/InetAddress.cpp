//
// Created by rjd67 on 2020/7/17.
//

#include <arpa/inet.h>
#include <netinet/in.h>

#include "InetAddress.h"

using namespace higan;

InetAddress::InetAddress(int port):
		InetAddress("0.0.0.0", port)
{

}

InetAddress::InetAddress(const std::string& ip, int port):
		ip_(ip),
		port_(port)
{

}

void InetAddress::GetSockaddr(sockaddr* addr)
{
	struct sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);

	inet_pton(AF_INET, ip_.data(), &addr_in->sin_addr);
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port_);

}

std::string InetAddress::GetIpPort()
{
	return ip_ + ":" +std::to_string(port_);
}
