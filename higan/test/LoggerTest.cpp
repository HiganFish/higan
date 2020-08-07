//
// Created by rjd67 on 2020/8/6.
//

#include "higan/utils/Logger.h"

int main()
{
	higan::Logger::SetLogLevel(higan::Logger::DEBUG);

	LOG_DEBUG << higan::Fmt("%s: %s", "DEBUG", "DEBUG") << " 1";
	LOG_INFO << higan::Fmt("%s: %s", "INFO", "INFO") << " 1";
	LOG_WARN << higan::Fmt("%s: %s", "WARN", "WARN") << " 1";
	LOG_ERROR << higan::Fmt("%s: %s", "ERROR", "ERROR") << " 1";
	LOG_FATAL << higan::Fmt("%s: %s", "FATAL", "FATAL") << " 1";

	return 0;
}