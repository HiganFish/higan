//
// Created by rjd67 on 2020/7/30.
//

#include "FileContext.h"

using namespace higan;

FileContext::FileContext(const FileForRead::FileForReadPtr& file_ptr, bool close_connection):
		close_connection_(close_connection),
		sum_read_from_cache_(0),
		read_over_(false),
		file_ptr_(file_ptr)
{

}

FileContext::~FileContext()
{

}

ssize_t FileContext::ReadFileToBuffer(Buffer* buffer)
{
	if (!buffer || !file_ptr_)
	{
		return -1;
	}

	if (read_over_)
	{
		return 0;
	}

	ssize_t read_size = file_ptr_->ReadFileToBuffer(buffer);
	if (read_size > 0)
	{
		sum_read_from_cache_ += read_size;
		if (sum_read_from_cache_ == file_ptr_->GetFileSize())
		{
			read_over_ = true;
		}
	}

	return read_size;
}

bool FileContext::CloseConnection() const
{
	return close_connection_;
}