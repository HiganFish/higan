//
// Created by rjd67 on 2020/8/7.
//

#ifndef HIGAN_LOGFILE_H
#define HIGAN_LOGFILE_H

#include <ctime>
#include <memory>
#include <string>
#include "higan/base/Mutex.h"

namespace higan
{
class FileForAppend;
class LogFile
{
public:

	const static time_t DEFAULT_FLUSH_INTERVAL;
	const static time_t DEFAULT_ROLL_INTERVAL;
	const static size_t DEFAULT_MAX_FILE_SIZE;

	/**
	 * 创建日志文件管理
	 * @param log_dir 日志文件文件夹
	 * @param log_prefix 日志文件前缀
	 * @param thread_safe
	 */
	explicit LogFile(const std::string& log_dir, const std::string& log_prefix, bool thread_safe);
	~LogFile();

	void Flush();

	void Append(const char* str, size_t len);

private:

	std::unique_ptr<Mutex> mutex_;

	std::string log_dir_;
	std::string log_prefix_;

	time_t last_flush_time_;
	time_t last_roll_time_;

	std::unique_ptr<FileForAppend> file_ptr_;

	void RollFile();

	void AppendInternal(const char* str, size_t len);

	std::string GetFileName();
};
}

#endif //HIGAN_LOGFILE_H
