//
// Created by rjd67 on 2020/7/26.
//

#ifndef HIGAN_FILE_H
#define HIGAN_FILE_H

#include <string>
#include <sys/stat.h>

#include "higan/Buffer.h"

namespace higan
{
class File
{
public:
	explicit File(const std::string& file_path_);
	~File();

	size_t GetFileSize() const;

	static size_t GetFileSize(const std::string& file_path_);

	/**
	 * 从文件中读取字节
	 * @param buffer 存储字节
	 * @return 成功返回读取字节数 失败返回-1
	 */
	ssize_t ReadFileToBuffer(Buffer* buffer);
private:
	struct stat file_status_;

	int file_fd_;


};
}

#endif //HIGAN_FILE_H
