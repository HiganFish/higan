//
// Created by rjd67 on 2020/7/26.
//
#include <fcntl.h>

#include "File.h"

using namespace higan;

File::File(const std::string& file_path_):
		file_stat_(),
		file_status_(FileStatus::NOT_EXIST),
		file_fd_(-1)
{
	if (stat(file_path_.c_str(), &file_stat_) == 0)
	{

		if (S_ISDIR(file_stat_.st_mode))
		{
			file_status_ = FileStatus::IS_DIR;
		}
		else
		{
			file_fd_ = open(file_path_.c_str(), O_RDONLY | O_CLOEXEC);
			if (file_fd_ == -1)
			{
				file_status_ = FileStatus::FILE_OPEN_ERROR;
			}
			else
			{
				file_status_ = FileStatus::FILE_OPEN_SUCCESS;
			}
		}
	}
}

File::~File()
{
	close(file_fd_);
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
	return buffer->ReadFromFd(file_fd_);
}

File::FileStatus File::GetFileStatus() const
{
	return file_status_;
}

