//
// Created by rjd67 on 2020/10/14.
//

#include "UserManager.h"

UserManager::UserManager()
{

}

ELoginResult UserManager::Login(const std::string& username, const std::string& password)
{
	auto find_result = maps_.find(username);

	if (find_result == maps_.end())
	{
		return USER_NOT_EXIST;
	}
	else if (password != find_result->second)
	{
		return PASSWORD_WRONG;
	}
	else
	{
		return LOGIN_SUCCESS;
	}
}

ELoginResult UserManager::Register(const std::string& username, const std::string& password)
{
	auto find_result = maps_.find(username);

	if (find_result != maps_.end())
	{
		return USER_EXIST;
	}

	maps_[username] = password;

	return REGISTER_SUCCESS;
}
