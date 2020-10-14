//
// Created by rjd67 on 2020/10/14.
//

#ifndef HIGAN_USERMANAGER_H
#define HIGAN_USERMANAGER_H

#include <map>
#include <string>

#include "LoginNetworkPackage.h"
class UserManager
{
public:
	UserManager();
	~UserManager() = default;

	ELoginResult Login(const std::string& username,
			const std::string& password);

	ELoginResult Register(const std::string& username,
			const std::string& password);
private:

	std::map<std::string, std::string> maps_;
};


#endif //HIGAN_USERMANAGER_H
