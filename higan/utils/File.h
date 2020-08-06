//
// Created by rjd67 on 2020/7/26.
//

#ifndef HIGAN_FILE_H
#define HIGAN_FILE_H

#include <string>
#include <sys/stat.h>
#include <memory>

#include "higan/Buffer.h"

namespace higan
{
class File
{
public:

	enum class FileStatus
	{
		// 文件不存在
		NOT_EXIST,
		// 文件为文件夹
		IS_DIR,
		// 文件非文件夹 打开成功
		FILE_OPEN_SUCCESS,
		// 文件非文件夹 打开失败
		FILE_OPEN_ERROR,
		FILE_CACHING
	};

	typedef std::shared_ptr<File> FilePtr;

	/**
	 * 创建文件
	 * @param file_path_ 文件路径
	 * @param cache_max_size 可进行缓存的最大文件大小 默认为0 不进行缓存
	 */
	explicit File(const std::string& file_path, size_t cache_max_size = 0);
	~File();

	size_t GetFileSize() const;

	/**
	 * 从文件中读取字节
	 * @param buffer 存储字节
	 * @return 成功返回读取字节数 失败返回-1
	 */
	ssize_t ReadFileToBuffer(Buffer* buffer);

	FileStatus GetFileStatus() const;

	std::string ReadLine();

	void Append(const std::string& data);
	void Append(const char* data, size_t len);

private:

	std::string file_path_;

	struct stat file_stat_;

	FileStatus file_status_;

	int read_fd_;
	int write_fd_;

	/**
	 * 超过此大小的文件 不进行缓存 默认为0 不进行缓存
	 */
	size_t cache_max_size_;

	Buffer cache_buffer_;

	/**
	 * 尝试缓存文件 成功返回true 失败 false
	 */
	bool TryToCacheFile();
};
}

#endif //HIGAN_FILE_H
