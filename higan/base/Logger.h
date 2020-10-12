//
// Created by rjd67 on 2020/7/16.
//

#ifndef HIGAN_LOGGER_H
#define HIGAN_LOGGER_H

#include <cstdio>
#include <functional>
#include <string>

namespace higan
{

const static size_t G_BUFFER_DEFAULT_SIZE = 512;

class TinyBuffer
{
public:
	TinyBuffer();

	~TinyBuffer();

	/**
	 * 获取读位置指针
	 * @return
	 */
	const char* ReadBegin();

	/**
	 * 获取写位置指针
	 * @return
	 */
	char* WriteBegin();

	/**
	 * 向前移动读指针 最大不能超过写指针位置
	 * @param index
	 */
	void AddReadIndex(size_t index);

	/**
	 * 向前移动写指针 最大不超过 G_BUFFER_DEFAULT_SIZE
	 * @param index
	 */
	void AddWriteIndex(size_t index);

	size_t ReadableSize() const;

	size_t WritableSize() const;

private:
	char buffer_[G_BUFFER_DEFAULT_SIZE];

	size_t read_idx_;

	size_t write_idx_;
};

class Fmt
{
public:
	Fmt(const char* format, ...);

	const char* CStr() const;
private:
	char buffer_[256];
};


class Logger
{
public:

	typedef std::function<void(const char* data, size_t len)> OutputFunction;
	typedef std::function<void()> FlushFunction;

	enum LogLevel
	{
		DEBUG = 0,
		INFO = 1,
		WARN = 2,
		ERROR = 3,
		FATAL = 4
	};

	Logger(LogLevel level, const char* full_file_name, int line, const char* func_name);
	~Logger();

//	void Printf(const char* format, ...);

	/**
	 * 将日志记录到 文件中
	 * @param log_dir 文件路径及前缀 如/root/test 将自动在test后 增加时间和后缀 构成文件名
	 * @param thread_safe false 不使用线程安全机制
	 */
	static void SetLogToFile(const std::string& log_dir, const std::string& log_prefix, bool thread_safe);
	static void SetLogToConsole();

	static void SetOutputFunction(const OutputFunction& func);
	static void SetFlushFunction(const FlushFunction& func);
	static void SetLogLevel(LogLevel level);
	static Logger::LogLevel GetLogLevel();

	Logger& operator<<(const char* str);
	Logger& operator<<(const Fmt& fmt);
	Logger& operator<<(const std::string& str);

private:

	TinyBuffer buffer_;

	LogLevel level_;
};

/**
 * #define LOG_DEBUG(format, ...)	\
 * if (higan::Logger::GetLogLevel() <= higan::Logger::LOG_DEBUG)	\
 * higan::Logger(higan::Logger::LOG_DEBUG, __FILE__, __LINE__, __FUNCTION__).Printf(format, __VA_ARGS__)
 *
 * 本来提供的上面样式的记录宏 然而如果只是单纯打印一条纯文本 就还要再额外定义一个宏  因为宏命令没有办法重载
 *
 * 本来想着不用muduo的设计样式 最终还是用了 真香.........
 */


#define LOG_DEBUG	\
if (higan::Logger::GetLogLevel() <= higan::Logger::DEBUG)	\
higan::Logger(higan::Logger::DEBUG, __FILE__, __LINE__, __FUNCTION__)

#define LOG_INFO	\
if (higan::Logger::GetLogLevel() <= higan::Logger::INFO)	\
higan::Logger(higan::Logger::INFO, __FILE__, __LINE__, __FUNCTION__)

#define LOG_WARN	\
if (higan::Logger::GetLogLevel() <= higan::Logger::WARN)	\
higan::Logger(higan::Logger::WARN, __FILE__, __LINE__, __FUNCTION__)

#define LOG_ERROR	\
if (higan::Logger::GetLogLevel() <= higan::Logger::ERROR)	\
higan::Logger(higan::Logger::ERROR, __FILE__, __LINE__, __FUNCTION__)

#define LOG_FATAL	\
higan::Logger(higan::Logger::FATAL, __FILE__, __LINE__, __FUNCTION__)


//#define LOG_DEBUG_P(format, ...) LOG_DEBUG.Printf(format, __VA_ARGS__);
//
//#define LOG_INFO_P(format, ...) LOG_INFO.Printf(format, __VA_ARGS__);
//
//#define LOG_WARN_P(format, ...)	LOG_WARN.Printf(format, __VA_ARGS__);
//
//#define LOG_ERROR_P(format, ...) LOG_ERROR.Printf(format, __VA_ARGS__);
//
//#define LOG_FATAL_P(format, ...) LOG_FATAL.Printf(format, __VA_ARGS__);

}
#endif