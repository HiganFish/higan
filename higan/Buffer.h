//
// Created by rjd67 on 2020/7/20.
//

#ifndef HIGAN_BUFFER_H
#define HIGAN_BUFFER_H

#include <vector>
#include <unistd.h>
#include <string>

namespace higan
{
class Buffer
{
public:

	static const int DEFAULT_BUFFER_SIZE;
	static const int DEFAULT_READ_INDEX;
	static const int DEFAULT_WRITE_INDEX;
	static const int MAX_EX_BUFFER_SIZE;

	Buffer();
	~Buffer();

	/**
	 * 从文件描述符中读取内容
	 * @param fd 文件描述符
	 * @return 成功返回读取字节数 失败返回-1
	 */
	ssize_t ReadFromFd(int fd);

	/**
	 * 获得buffer的读指针
	 * @return
	 */
	char* ReadBegin();

	/**
	 * 获得buffer的写指针
	 * @return
	 */
	char* WriteBegin();

	/**
	 * @return 可读字节数
	 */
	size_t ReadableSize() const;

	/**
	 * @return 可写字节数
	 */
	size_t WritableSize() const;

	void AddReadIndex(size_t index);
	void AddWriteIndex(size_t index);

	std::string ReadAllAsString();

	void Append(const char* begin, const char* end);

	void Append(const char* begin, size_t len);

	void Append(const std::string& data);

	void Append(Buffer* buffer);

	void AppendCRLF();

	/**
	 * 将Buffer恢复为初始状态 清除所有数据
	 */
	void Reset();
private:

	const static char CRLF[];
	const static size_t CRLF_LEN;

	std::vector<char> buffer_;

	/**
	 * 读下标 防止迭代器失效使用下标
	 */
	size_t read_idx_;

	/**
	 * 写下标 防止迭代器失效使用下标
	 */
	size_t write_idx_;

	/**
	 * 从字符数组中复制ex_len字节的内容
	 * @param exbuffer
	 * @param ex_len
	 */
	void CopyExDataToBuffer(const char* exbuffer, size_t ex_len);
};
}

#endif //HIGAN_BUFFER_H
