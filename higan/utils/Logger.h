//
// Created by rjd67 on 2020/7/16.
//

#ifndef HIGAN_LOGGER_H
#define HIGAN_LOGGER_H

#include <cstdio>
#include <functional>

namespace higan
{

const static size_t G_BUFFER_DEFAULT_SIZE = 512;

class TinyBuffer
{
public:
	TinyBuffer():
			buffer_(),
			read_idx_(),
			write_idx_()
	{

	}
	~TinyBuffer()
	{

	}

	/**
	 * 获取读位置指针
	 * @return
	 */
	char* ReadBegin()
	{
		return &buffer_[read_idx_];
	}

	/**
	 * 获取写位置指针
	 * @return
	 */
	char* WriteBegin()
	{
		return &buffer_[write_idx_];
	}

	/**
	 * 向前移动读指针 最大不能超过写指针位置
	 * @param index
	 */
	void AddReadIndex(size_t index)
	{
		read_idx_ = std::min(read_idx_ + index, write_idx_);
	}

	/**
	 * 向前移动写指针 最大不超过 G_BUFFER_DEFAULT_SIZE
	 * @param index
	 */
	void AddWriteIndex(size_t index)
	{
		write_idx_ = std::min(write_idx_ + index, G_BUFFER_DEFAULT_SIZE);
	}

	size_t ReadableSize() const
	{
		return write_idx_ - read_idx_;
	}

	size_t WritableSize() const
	{
		return G_BUFFER_DEFAULT_SIZE - write_idx_;
	}

private:
	char buffer_[G_BUFFER_DEFAULT_SIZE];

	size_t read_idx_;

	size_t write_idx_;
};


class Logger
{
public:

	typedef std::function<void(const char* data, size_t len)> OutputFunction;
	typedef std::function<void()> FlushFunction;

	enum LogLevel
	{
		LOG_DEBUG = 0,
		LOG_INFO = 1,
		LOG_WARN = 2,
		LOG_ERROR = 3,
		LOG_FATAL = 4
	};

	Logger(LogLevel level, const char* full_file_name, int line, const char* func_name);
	~Logger();

	void Log(const char* format, ...);

	static void SetOutputFunction(const OutputFunction& func);
	static void SetFlushFunction(const FlushFunction& func);
	static void SetLogLevel(LogLevel level);
	static Logger::LogLevel GetLogLevel();

private:

	TinyBuffer buffer_;

	LogLevel level_;
};

#define DEBUG(format, ...)	\
if (higan::Logger::GetLogLevel() <= higan::Logger::LOG_DEBUG)	\
higan::Logger(higan::Logger::LOG_DEBUG, __FILE__, __LINE__, __FUNCTION__).Log(format, __VA_ARGS__)

#define INFO(format, ...)	\
if (higan::Logger::GetLogLevel() <= higan::Logger::LOG_INFO)	\
higan::Logger(higan::Logger::LOG_INFO, __FILE__, __LINE__, __FUNCTION__).Log(format, __VA_ARGS__)

#define WARN(format, ...)	\
if (higan::Logger::GetLogLevel() <= higan::Logger::LOG_WARN)	\
higan::Logger(higan::Logger::LOG_WARN, __FILE__, __LINE__, __FUNCTION__).Log(format, __VA_ARGS__)

#define ERROR(format, ...)	\
if (higan::Logger::GetLogLevel() <= higan::Logger::LOG_ERROR)	\
higan::Logger(higan::Logger::LOG_ERROR, __FILE__, __LINE__, __FUNCTION__).Log(format, __VA_ARGS__)

#define FATAL(format, ...)	\
if (higan::Logger::GetLogLevel() <= higan::Logger::LOG_FATAL)	\
higan::Logger(higan::Logger::LOG_FATAL, __FILE__, __LINE__, __FUNCTION__).Log(format, __VA_ARGS__)

}

#endif //HIGAN_LOGGER_H
