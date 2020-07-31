//
// Created by rjd67 on 2020/7/30.
//

#include "FileContext.h"

using namespace higan;

FileContext::FileContext(const std::string& url, bool close_connection):
		url_(url),
		close_connection_(close_connection),
		file_(url_)
{

}

FileContext::~FileContext()
{

}

ssize_t FileContext::ReadFileToBuffer(Buffer* buffer)
{
	return file_.ReadFileToBuffer(buffer);
}

bool FileContext::CloseConnection() const
{
	return close_connection_;
}