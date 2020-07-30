//
// Created by rjd67 on 2020/7/30.
//

#include "FileContext.h"

using namespace higan;

FileContext::FileContext(const std::string& url, bool keep_connection):
		url_(url),
		keep_connection_(keep_connection),
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

bool FileContext::IsKeepConnection() const
{
	return keep_connection_;
}