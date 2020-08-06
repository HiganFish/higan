//
// Created by rjd67 on 2020/7/16.
//

#include <string>
#include <unistd.h>
#include <stdarg.h>
#include <cstring>
#include "higan/utils/Logger.h"
#include "higan/utils/TimeStamp.h"

using namespace higan;

const char* G_LEVEL_TO_STRING[]  = { "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"};


void DefaultOutputFunc(const char* data, size_t len)
{
	fwrite(data, 1, len, stdout);
}

void DefaultFlushFunction()
{
	fflush(stdout);
}

/**
 * 在这里设置全局变量 而不是 加在对象里面
 * 难道是因为Logger会频繁地构造析构 减少资源消耗?
 * 毕竟这两函数 只会在程序运行开始修改
 */
Logger::OutputFunction g_output_func = DefaultOutputFunc;
Logger::FlushFunction g_flush_func = DefaultFlushFunction;
Logger::LogLevel g_log_level = higan::Logger::LOG_INFO;

Logger::Logger(LogLevel level, const char* full_file_name, int line, const char* func_name):
		level_(level)
{
	std::string thread_id_;

	pid_t pid = getpid();

	TimeStamp stamp(TimeStamp::Now());
	buffer_.AddWriteIndex(stamp.FormatToBuffer(buffer_.WriteBegin(), buffer_.WritableSize()));

	const char* file_name = strrchr(full_file_name, '/');
	if (file_name)
	{
		file_name += 1;
	}
	else
	{
		file_name = full_file_name;
	}

	buffer_.AddWriteIndex(snprintf(buffer_.WriteBegin(), buffer_.WritableSize(),
			" [pid: %d] [%s] [%s:%d - %s] ",
			pid,
			G_LEVEL_TO_STRING[level],
			file_name,
			line,
			func_name));
}

Logger::~Logger()
{
	g_output_func(buffer_.ReadBegin(), buffer_.ReadableSize());

	if (level_ == LOG_FATAL)
	{
		g_flush_func();
		abort();
	}
}

void Logger::Log(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	buffer_.AddWriteIndex(vsnprintf(buffer_.WriteBegin(), buffer_.WritableSize(), format, arg));
	va_end(arg);

	*buffer_.WriteBegin() = '\n';
	buffer_.AddWriteIndex(1);

}

void Logger::SetOutputFunction(const Logger::OutputFunction& func)
{
	g_output_func = func;
}

void Logger::SetFlushFunction(const Logger::FlushFunction& func)
{
	g_flush_func = func;
}

void Logger::SetLogLevel(Logger::LogLevel level)
{
	g_log_level = level;
}

Logger::LogLevel Logger::GetLogLevel()
{
	return g_log_level;
}
