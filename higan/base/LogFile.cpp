//
// Created by rjd67 on 2020/8/7.
//

#include "higan/base/LogFile.h"
#include "higan/base/File.h"
#include "higan/base/System.h"

#include <memory>
#include "TimeStamp.h"

using namespace higan;

const time_t LogFile::DEFAULT_FLUSH_INTERVAL = 3;
const time_t LogFile::DEFAULT_ROLL_INTERVAL = 24 * 60 * 60;
const size_t LogFile::DEFAULT_MAX_FILE_SIZE = 10 * 1024 * 1024;

LogFile::LogFile(const std::string& log_dir, const std::string& log_prefix, bool thread_safe) :
		mutex_(thread_safe ? std::make_unique<Mutex>() : nullptr),
		log_dir_(log_dir),
		log_prefix_(log_prefix),
		last_flush_time_(time(nullptr)),
		last_roll_time_(time(nullptr)),
		file_ptr_(nullptr)
{
	RollFile();
}

LogFile::~LogFile()
{

}

void LogFile::Flush()
{
	last_flush_time_ = time(nullptr);
	file_ptr_->Flush();
}

void LogFile::Append(const char* str, size_t len)
{
	if (mutex_)
	{
		MutexLockGuard guard(*mutex_);
		AppendInternal(str, len);
	}
	else
	{
		AppendInternal(str, len);
	}
}

void LogFile::RollFile()
{
	bool result = System::MakeDirIfNotExist(log_dir_, true);
	if (!result)
	{
		fprintf(stderr, "create log dir failed\n");
	}

	std::string file_name = GetFileName();
	file_ptr_ = std::make_unique<FileForAppend>(file_name);
}

void LogFile::AppendInternal(const char* str, size_t len)
{
	time_t now = time(nullptr);

	file_ptr_->Append(str, len);

	if (now > last_flush_time_ + DEFAULT_FLUSH_INTERVAL)
	{
		Flush();
		last_flush_time_ = now;
	}

	if (now > last_roll_time_ + DEFAULT_ROLL_INTERVAL)
	{
		RollFile();
		last_roll_time_ = now;
	}

	if (file_ptr_->GetWriteSumBytes() > DEFAULT_MAX_FILE_SIZE)
	{
		RollFile();
	}
}

std::string LogFile::GetFileName()
{
	time_t now = time(nullptr);

	struct tm* tm_now = localtime(&now);

	char buffer[32];

	strftime(buffer, sizeof buffer, ".%Y%m%d-%H%M%S.", tm_now);

	std::string result_name = log_dir_ + "/" + log_prefix_;
	result_name.append(buffer);
	result_name += "log";

	return result_name;
}
