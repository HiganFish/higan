//
// Created by rjd67 on 2020/7/29.
//

#include <higan/base/Codec.h>
#include <higan/base/Logger.h>

int main()
{
	char url1[] = "%E7%BD%91%E7%BB%9C%E9%85%8D%E7%BD%AE";
	char url2[] = "E7BD91EB99858DE7BDAE";

	LOG_INFO << higan::Codec::DecodeUrl(url1, strlen(url1));
	LOG_INFO << higan::Codec::DecodeUrl(url2, strlen(url2));

	return 0;
}