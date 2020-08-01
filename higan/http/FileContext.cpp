//
// Created by rjd67 on 2020/7/30.
//

#include "FileContext.h"

using namespace higan;

FileContext::FileContext(const File::FilePtr& file_ptr, bool close_connection):
		file_ptr_(file_ptr),
		close_connection_(close_connection)
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

	return file_ptr_->ReadFileToBuffer(buffer);
}

bool FileContext::CloseConnection() const
{
	return close_connection_;
}