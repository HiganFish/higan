//
// Created by rjd67 on 2020/7/26.
//
#include <fcntl.h>

#include "File.h"

using namespace higan;

File::File(const std::string& file_path_)
{
	stat(file_path_.c_str(), &file_status_);
	file_fd_ = open(file_path_.c_str(), O_RDONLY | O_CLOEXEC);
}

File::~File()
{
	close(file_fd_);
}

size_t File::GetFileSize() const
{
	return file_status_.st_size;
}

size_t File::GetFileSize(const std::string& file_path_)
{
	struct stat file_status;
	stat(file_path_.c_str(), &file_status);

	return static_cast<size_t>(file_status.st_size);
}

ssize_t File::ReadFileToBuffer(Buffer* buffer)
{
	if (!buffer)
	{
		return -1;
	}
	return buffer->ReadFromFd(file_fd_);
}


