//
// Created by rjd67 on 2020/7/26.
//
#include <fcntl.h>

#include "higan/utils/File.h"
#include "Logger.h"

using namespace higan;

File::File(const std::string& file_path, size_t cache_max_size):
		file_path_(file_path),
		file_stat_(),
		file_status_(FileStatus::NOT_EXIST),
		read_fd_(-1),
		write_fd_(-1),
		cache_max_size_(cache_max_size),
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
				file_status_ = FileStatus::FILE_OPEN_ERROR;
			}
			else
			{
				file_status_ = FileStatus::FILE_OPEN_SUCCESS;
				if (TryToCacheFile())
				{
					file_status_ = FileStatus::FILE_CACHING;
					close(read_fd_);
					read_fd_ = -1;
				}
			}
		}
	}
}

File::~File()
{
	if (read_fd_ != -1)
	{
		close(read_fd_);
	}
}

size_t File::GetFileSize() const
{
	return file_stat_.st_size;
}

ssize_t File::ReadFileToBuffer(Buffer* buffer)
{
	if (!buffer)
	{
		return -1;
	}

	size_t result = -1;
	if (file_status_ == FileStatus::FILE_OPEN_SUCCESS)
	{
		result = buffer->ReadFromFd(read_fd_);
	}
	else if (file_status_ == FileStatus::FILE_CACHING)
	{
		buffer->CopyFromBuffer(&cache_buffer_);
		result = buffer->ReadableSize();
	}
	else
	{
		result = -1;
	}

	return result;
}

File::FileStatus File::GetFileStatus() const
{
	return file_status_;
}

bool File::TryToCacheFile()
{
	if (GetFileSize() <= cache_max_size_)
	{
		ssize_t read_size = -1;

		while (true)
		{
			read_size = cache_buffer_.ReadFromFd(read_fd_);
			if (read_size == 0)
			{
				break;
			}
			else if (read_size < 0)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

std::string File::ReadLine()
{
	if (ReadFileToBuffer(&cache_buffer_) == -1)
	{
		return "";
	}

	return cache_buffer_.ReadLine();
}

void File::Append(const std::string& data)
{
	Append(data.c_str(), data.length());
}

void File::Append(const char* data, size_t len)
{
	if (write_fd_ == -1)
	{
		write_fd_ = open(file_path_.c_str(), O_CREAT | O_WRONLY | O_APPEND);

		if (write_fd_ == -1)
		{
			LOG_ERROR << higan::Fmt("open file: %s for write error", file_path_.c_str());
		}
	}

	if (write_fd_ != -1)
	{
		ssize_t write_result = write(write_fd_, data, len);

		if (write_result < 0)
		{
			LOG_ERROR << higan::Fmt("write to file: %s error", file_path_.c_str());
		}
	}
}

