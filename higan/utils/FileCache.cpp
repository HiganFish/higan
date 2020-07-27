//
// Created by rjd67 on 2020/7/26.
//

#include "FileCache.h"

using namespace higan;

FileCache::FileCache()
{

}

FileCache::~FileCache()
{

}


FileCache::FilePtr FileCache::GetFile(const std::string& file_path)
{
//	FilePtr file_ptr;
//	if (enable_file_cache_)
//	{
//		auto find_result = file_ptr_map_.find(file_path);
//		if (find_result == file_ptr_map_.end())
//		{
//			file_ptr = std::make_shared<File>(file_path);
//			file_ptr_map_.insert({file_path, file_ptr});
//		}
//		else
//		{
//			file_ptr = find_result->second;
//		}
//	}
//	else
//	{
//		file_ptr = std::make_shared<File>(file_path);
//	}

	return std::make_shared<File>(file_path);
}
