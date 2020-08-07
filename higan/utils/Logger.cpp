//
// Created by rjd67 on 2020/7/16.
//

#include <string>
#include <cstdarg>
#include <cstring>
#include "higan/utils/Logger.h"
#include "higan/utils/TimeStamp.h"
#include "higan/utils/System.h"
#include "higan/utils/LogFile.h"

using namespace higan;

TinyBuffer::TinyBuffer():
		buffer_(),
		read_idx_(),
		write_idx_()
{
}
TinyBuffer::~TinyBuffer()
{
}

/**
 * 获取读位置指针
 * @return
 */
const char* TinyBuffer::ReadBegin()
{
	return &buffer_[read_idx_];
}

/**
 * 获取写位置指针
 * @return
 */
char* TinyBuffer::WriteBegin()
{
	return &buffer_[write_idx_];
}

/**
 * 向前移动读指针 最大不能超过写指针位置
 * @param index
 */
void TinyBuffer::AddReadIndex(size_t index)
{
	read_idx_ = std::min(read_idx_ + index, write_idx_);
}

/**
 * 向前移动写指针 最大不超过 G_BUFFER_DEFAULT_SIZE
 * @param index
 */
void TinyBuffer::AddWriteIndex(size_t index)
{
	write_idx_ = std::min(write_idx_ + index, G_BUFFER_DEFAULT_SIZE);
}

size_t TinyBuffer::ReadableSize() const
{
	return write_idx_ - read_idx_;
}

size_t TinyBuffer::WritableSize() const
{
	return G_BUFFER_DEFAULT_SIZE - write_idx_;
}

Fmt::Fmt(const char* format, ...):
		buffer_()
{
	va_list args;
	va_start(args, format);
	vsnprintf(buffer_, sizeof buffer_, format, args);
	va_end(args);
}

const char* Fmt::CStr() const
{
	return buffer_;
}


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
Logger::LogLevel g_log_level = Logger::INFO;
std::shared_ptr<LogFile> g_log_file;

void Logger::SetOutputFunction(const Logger::OutputFunction& func)
{
	g_output_func = func;
}

void Logger::SetFlushFunction(const Logger::FlushFunction& func)
{
	g_flush_func = func;
}

void Logger::SetLogToFile(const std::string& base_name, bool thread_safe)
{
	g_log_file = std::make_shared<LogFile>(base_name, thread_safe);
	SetOutputFunction(std::bind(&LogFile::Append, g_log_file,
			std::placeholders::_1, std::placeholders::_2));
	SetFlushFunction(std::bind(&LogFile::Flush, g_log_file));
}

void Logger::SetLogToConsole()
{
	if (g_log_file)
	{
		g_log_file.reset();
	}
}

void Logger::SetLogLevel(Logger::LogLevel level)
{
	g_log_level = level;
}

Logger::LogLevel Logger::GetLogLevel()
{
	return g_log_level;
}

const char* G_LEVEL_TO_STRING[]  = { "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"};

Logger::Logger(LogLevel level, const char* full_file_name, int line, const char* func_name):
		level_(level)
{
	std::string thread_id_;

	pid_t pid = System::GetTid();

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
			G_LEVEL_TO_STRING[static_cast<int>(level)],
			file_name,
			line,
			func_name));
}

//void Logger::Printf(const char* format, ...)
//{
//	va_list arg;
//	va_start(arg, format);
//	buffer_.AddWriteIndex(vsnprintf(buffer_.WriteBegin(), buffer_.WritableSize(), format, arg));
//	va_end(arg);
//}

Logger::~Logger()
{
	*buffer_.WriteBegin() = '\n';
	buffer_.AddWriteIndex(1);

	g_output_func(buffer_.ReadBegin(), buffer_.ReadableSize());

	if (level_ == FATAL)
	{
		g_flush_func();
		abort();
	}
}

Logger& Logger::operator<<(const char* str)
{
	buffer_.AddWriteIndex(snprintf(buffer_. WriteBegin(), buffer_.ReadableSize(),
			"%s", str));

	return *this;
}

Logger& Logger::operator<<(const Fmt& fmt)
{
	return *this << fmt.CStr();
}

Logger& Logger::operator<<(const std::string& str)
{
	return *this << str.c_str();
}
