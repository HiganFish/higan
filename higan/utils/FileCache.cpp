//
// Created by rjd67 on 2020/8/4.
//

#include "higan/utils/FileCache.h"
#include "higan/utils/Logger.h"

using namespace higan;

FileCache::FileCache(const std::string& name, size_t max_file_size):
	name_(name),
	max_file_size_(max_file_size),
	mutex_()
{

}

FileCache::~FileCache()
{

}

File::FilePtr FileCache::GetFilePtr(const std::string& url)
{

	MutexLockGuard guard(mutex_);

	auto find_result = file_ptr_map_.find(url);
	if (find_result == file_ptr_map_.end())
	{
		File::FilePtr result;
		result = std::make_shared<File>(url, max_file_size_);
		if (result->GetFileStatus() == File::FileStatus::FILE_CACHING)
		{
			LOG_INFO << higan::Fmt("cache@%s cache file: %s", name_.c_str(), url.c_str());
			file_ptr_map_[url] = result;
		}
		return result;
	}
	else
	{
		LOG_INFO << higan::Fmt("cache@%s hit cache file: %s", name_.c_str(), url.c_str());
		return find_result->second;
	}
}

void FileCache::SetMaxFileSize(size_t file_size)
{
	max_file_size_ = file_size;
}
