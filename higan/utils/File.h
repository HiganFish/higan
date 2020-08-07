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
class FileForRead
{
public:

	enum class FileStatus
	{
		// 文件不存在
		NOT_EXIST,
		// 文件为文件夹
		IS_DIR,
		// 文件非文件夹 打开成功
		OPEN_SUCCESS,
		// 文件非文件夹 打开失败
		OPEN_ERROR
	};

	typedef std::shared_ptr<FileForRead> FileForReadPtr;

	/**
	 * 创建文件
	 * @param file_path_ 文件路径
	 */
	explicit FileForRead(const std::string& file_path);

	~FileForRead();

	size_t GetFileSize() const;

	/**
	 * 从文件中读取字节
	 * @param buffer 存储字节
	 * @return 成功返回读取字节数 失败返回-1
	 */
	ssize_t ReadFileToBuffer(Buffer* buffer);

	FileStatus GetFileStatus() const;

	std::string ReadLine();

private:

	std::string file_path_;

	struct stat file_stat_;

	FileStatus file_status_;

	int read_fd_;

	Buffer cache_buffer_;
};

class FileForAppend
{
public:
	explicit FileForAppend(const std::string& url);
	~FileForAppend();

	void Append(const std::string& data);

	void Append(const char* data, size_t len);

	void Flush();

	size_t GetWriteSumBytes() const;
private:

	std::string url_;

	FILE* file_fp_;

	char buffer[32 * 1024];

	size_t Write(const char* data, size_t len);

	size_t write_sum_bytes_;
}
}
#endif //HIGAN_FILE_H
