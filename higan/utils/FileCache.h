//
// Created by rjd67 on 2020/8/4.
//

#ifndef HIGAN_FILECACHE_H
#define HIGAN_FILECACHE_H

#include <string>
#include <map>

#include "higan/utils/File.h"
#include "higan/base/Mutex.h"


namespace higan
{
class FileCache
{
public:
	/**
	 *
	 * @param name 文件缓冲管理名称
	 * @param max_file_size 小于此大小的文件将被缓存
	 */
	FileCache(const std::string& name, size_t max_file_size);
	~FileCache();


	File::FilePtr GetFilePtr(const std::string& url);

	void SetMaxFileSize(size_t file_size);
private:

	std::string name_;
	size_t max_file_size_;
	Mutex mutex_;

	std::map<std::string, File::FilePtr> file_ptr_map_;
};
}

#endif //HIGAN_FILECACHE_H
