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
		FILE_OPEN_ERROR
	};

	typedef std::shared_ptr<File> FilePtr;

	explicit File(const std::string& file_path_);
	~File();

	size_t GetFileSize() const;

	/**
	 * 从文件中读取字节
	 * @param buffer 存储字节
	 * @return 成功返回读取字节数 失败返回-1
	 */
	ssize_t ReadFileToBuffer(Buffer* buffer);

	FileStatus GetFileStatus() const;

private:
	struct stat file_stat_;

	FileStatus file_status_;

	int file_fd_;
};
}

#endif //HIGAN_FILE_H
