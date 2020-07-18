//
// Created by rjd67 on 2020/7/17.
//



#include "InetAddress.h"

using namespace higan;

InetAddress::InetAddress():
	InetAddress("FF.FF.FF.FF", -1)
{

}

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


void InetAddress::SetSockaddr(sockaddr_in* addr)
{
	char ip[20];
	inet_ntop(AF_INET, &addr->sin_addr, ip, static_cast<socklen_t>(sizeof ip));

	ip_ = ip;

	port_ = ntohs(addr->sin_port);
}

std::string InetAddress::GetIpPort() const
{
	return ip_ + ":" +std::to_string(port_);
}
