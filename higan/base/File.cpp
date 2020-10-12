//
// Created by rjd67 on 2020/7/26.
//
#include <fcntl.h>
#include <cassert>
#include <cstring>

#include "higan/base/File.h"
#include "Logger.h"

using namespace higan;

FileForRead::FileForRead(const std::string& file_path):
		file_path_(file_path),
		file_stat_(),
		file_status_(FileStatus::NOT_EXIST),
		read_fd_(-1),
		cache_buffer_()
{
	if (stat(file_path_.c_str(), &file_stat_) == 0)
	{

		if (S_ISDIR(file_stat_.st_mode))
		{
			file_status_ = FileStatus::IS_DIR;
		}
		else
		{
			read_fd_ = open(file_path_.c_str(), O_RDONLY | O_CLOEXEC);
			if (read_fd_ == -1)
			{
				file_status_ = FileStatus::OPEN_ERROR;
			}
			else
			{
				file_status_ = FileStatus::OPEN_SUCCESS;
			}
		}
	}
}

FileForRead::~FileForRead()
{
	if (read_fd_ != -1)
	{
		close(read_fd_);
	}
}

size_t FileForRead::GetFileSize() const
{
	return file_stat_.st_size;
}

ssize_t FileForRead::ReadFileToBuffer(Buffer* buffer)
{
	if (!buffer)
	{
		return -1;
	}

	size_t result = -1;
	if (file_status_ == FileStatus::OPEN_SUCCESS)
	{
		result = buffer->ReadFromFd(read_fd_);
	}
	else
	{
		result = -1;
	}

	return result;
}

FileForRead::FileStatus FileForRead::GetFileStatus() const
{
	return file_status_;
}

std::string FileForRead::ReadLine()
{
	if (ReadFileToBuffer(&cache_buffer_) == -1)
	{
		return "";
	}

	return cache_buffer_.ReadLine();
}


FileForAppend::FileForAppend(const std::string& url) :
		url_(url),
		file_fp_(fopen(url.c_str(), "ae")), // a O_WRONLY | O_CREAT | O_APPEND e O_CLOEXEC
		write_sum_bytes_(0)
{
	assert(file_fp_);
	setbuffer(file_fp_, buffer, sizeof buffer);
}

FileForAppend::~FileForAppend()
{
	Flush();
	fclose(file_fp_);
}

void FileForAppend::Append(const std::string& data)
{
	Append(data.c_str(), data.length());
}

void FileForAppend::Append(const char* data, size_t len)
{
	size_t has_writen = 0;
	/**
	 * 默认第一次 成功..
	 */
	size_t write_result = Write(data, len);

	has_writen += write_result;

	while (has_writen < len)
	{
		write_result = Write(data + has_writen, len - has_writen);
		if (write_result > 0)
		{
			has_writen += write_result;
		}
		else if (write_result == 0)
		{
			int error = ferror(file_fp_);
			if (error)
			{
				fprintf(stderr, "write file %s error error msg is: %s",
						url_.c_str(), strerror(error));
			}
			break;
		}
	}

	write_sum_bytes_ += has_writen;
}

void FileForAppend::Flush()
{
	fflush(file_fp_);
}

size_t FileForAppend::Write(const char* data, size_t len)
{
	return fwrite_unlocked(data, 1, len, file_fp_);
}

size_t FileForAppend::GetWriteSumBytes() const
{
	return write_sum_bytes_;
}
