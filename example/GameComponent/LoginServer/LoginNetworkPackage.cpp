//
// Created by rjd67 on 2020/10/13.
//

#include "LoginNetworkPackage.h"

uint32_t LoginNetworkPackage::login_number = 0;

LoginNetworkPackage::LoginNetworkPackage(std::string username, std::string password) :
		NetworkPackageBase(LOGIN, login_number++),
		authentication_key_(username + "#" + password)
{}

ssize_t LoginNetworkPackage::SerializeToBuffer(uint8_t* buffer, size_t BufferLen)
{
	ssize_t result = 0;

	ssize_t header_len = SerializeHeaderToBuffer(authentication_key_.length(), buffer, BufferLen);
	if (header_len <= 0)
	{
		return -1;
	}
	result += header_len;

	const uint8_t* authentication_key_data = (const uint8_t*)authentication_key_.c_str();

	memcpy(buffer + result, authentication_key_data, authentication_key_.length());
	result += authentication_key_.length();

	SerializeCheckSumToBuffer(buffer, result);
	return result;
}

ssize_t LoginNetworkPackage::DeSerializeFromBuffer(const uint8_t* buffer, size_t buffer_len)
{
	size_t body_len = 0;
	ssize_t parsed_len = DeSerializeHeaderFromBuffer(buffer, buffer_len, &body_len);

	if (parsed_len <= 0)
	{
		return parsed_len;
	}

	authentication_key_.append((char*)buffer + parsed_len, body_len);

	return parsed_len + body_len;
}

const std::string& LoginNetworkPackage::GetAuthenticationKey() const
{
	return authentication_key_;
}


LoginResultPackage::LoginResultPackage()
{

}

ssize_t LoginResultPackage::SerializeToBuffer(uint8_t* buffer, size_t BufferLen)
{
	return NetworkPackageBase::SerializeToBuffer(buffer, BufferLen);
}

ssize_t LoginResultPackage::DeSerializeFromBuffer(const uint8_t* buffer, size_t buffer_len)
{
	return ssize_t();
}

