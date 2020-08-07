//
// Created by rjd67 on 2020/8/7.
//

#ifndef HIGAN_LOGFILE_H
#define HIGAN_LOGFILE_H

#include <ctime>
#include <memory>
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

	explicit LogFile(const std::string& base_name, bool thread_safe);
	~LogFile();

	void Flush();

	void Append(const char* str, size_t len);

private:

	std::unique_ptr<Mutex> mutex_;

	std::string base_name_;

	time_t last_flush_time_;
	time_t last_roll_time_;

	std::unique_ptr<FileForAppend> file_ptr_;

	void RollFile();

	void AppendInternal(const char* str, size_t len);

	std::string GetFileName();
};
}

#endif //HIGAN_LOGFILE_H
