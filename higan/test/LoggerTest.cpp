//
// Created by rjd67 on 2020/8/6.
//

#include "higan/utils/Logger.h"

int main()
{
	DEBUG("%s: %s", "111", "222");

	higan::Logger::SetLogLevel(higan::Logger::LOG_DEBUG);

	DEBUG("%s: %s", "111", "222");
	INFO("%s: %s", "111", "222");
	WARN("%s: %s", "111", "222");
	ERROR("%s: %s", "111", "222");
	FATAL("%s: %s", "111", "222");

}