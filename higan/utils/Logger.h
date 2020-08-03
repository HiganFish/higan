//
// Created by rjd67 on 2020/7/16.
//

#ifndef HIGAN_LOGGER_H
#define HIGAN_LOGGER_H

#include <cstdio>
#include <cstdarg>
#include <cerrno>
#include <cstring>
#include <cstdlib>

inline void LOG_IF(bool r, const char* format, ...)
{
	if (r)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\nerrno: %d errormsg: %s\n", errno, strerror(errno));
	}
}

inline void LOG(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\n");
}

inline void EXIT_IF(bool r, const char* format, ...)
{
	if (r)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		abort();
	}
}

namespace higan
{
class Logger
{

};
}

#endif //HIGAN_LOGGER_H
