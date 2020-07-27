//
// Created by rjd67 on 2020/7/26.
//

#ifndef HIGAN_FILECACHE_H
#define HIGAN_FILECACHE_H

#include <map>
#include <memory>

#include "higan/utils/File.h"

namespace higan
{
class FileCache
{
public:
	typedef std::shared_ptr<File> FilePtr;
	typedef std::map<std::string, FilePtr> FilePtrMap;

	FileCache();
	~FileCache();

	FilePtr GetFile(const std::string& file_path);

private:
	FilePtrMap file_ptr_map_;

};
}

#endif //HIGAN_FILECACHE_H
