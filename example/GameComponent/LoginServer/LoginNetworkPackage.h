//
// Created by rjd67 on 2020/10/13.
//

#ifndef HIGAN_LOGINNETWORKPACKAGE_H
#define HIGAN_LOGINNETWORKPACKAGE_H

#include <NetworkPackageBase.h>
class LoginNetworkPackage : public NetworkPackageBase
{
public:

	LoginNetworkPackage() = default;
	LoginNetworkPackage(std::string username, std::string password);

	ssize_t SerializeToBuffer(uint8_t* buffer, size_t BufferLen) override;

	ssize_t DeSerializeFromBuffer(const uint8_t* buffer, size_t buffer_len) override;

	const std::string& GetAuthenticationKey() const;

private:
	static uint32_t login_number;

	std::string authentication_key_;
};

enum ELoginResult : uint8_t
{
	LOGIN_SUCCESS,
	USER_NOT_EXIST,
	PASSWORD_WRONG,
	REGISTER_SUCCESS,
	USER_EXIST
};

class LoginResultPackage : public NetworkPackageBase
{
public:
	LoginResultPackage();

	LoginResultPackage(ELoginResult result, uint32_t user_key);

	ssize_t SerializeToBuffer(uint8_t* buffer, size_t buffer_len) override;

	ssize_t DeSerializeFromBuffer(const uint8_t* buffer, size_t buffer_len) override;

	ELoginResult GetLoginResult() const;

	uint32_t GetUserId() const;

private:

	ELoginResult login_result_;
	uint32_t user_id_;
};

#endif //HIGAN_LOGINNETWORKPACKAGE_H
