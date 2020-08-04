//
// Created by rjd67 on 2020/7/20.
//

#include <sys/uio.h>
#include <cstring>

#include "higan/Buffer.h"

using namespace higan;

const int Buffer::DEFAULT_BUFFER_SIZE = 1024;
const int Buffer::DEFAULT_READ_INDEX = 8;
const int Buffer::DEFAULT_WRITE_INDEX = 8;
const int Buffer::MAX_EX_BUFFER_SIZE = 65535;
const char Buffer::CRLF[] = "\r\n";
const size_t Buffer::CRLF_LEN = strlen(CRLF);

Buffer::Buffer():
		Buffer(DEFAULT_BUFFER_SIZE)
{

}

Buffer::~Buffer()
{

}


Buffer::Buffer(int buffer_size):
		buffer_(buffer_size),
		read_idx_(DEFAULT_READ_INDEX),
		write_idx_(DEFAULT_WRITE_INDEX)

{

}

ssize_t Buffer::ReadFromFd(int fd)
{
	char exbuffer[MAX_EX_BUFFER_SIZE];

	iovec iovec_temp[2]{};
	iovec_temp[0].iov_base = WriteBegin();
	iovec_temp[0].iov_len = WritableSize();
	iovec_temp[1].iov_base = exbuffer;
	iovec_temp[1].iov_len = MAX_EX_BUFFER_SIZE;

	int iov_count = WritableSize() >= MAX_EX_BUFFER_SIZE ? 1 : 2;
	ssize_t read_size = readv(fd, iovec_temp, iov_count);

	if (read_size < 0)
	{
		return -1;
	}
	else if (read_size <= WritableSize())
	{
		AddWriteIndex(read_size);
	}
	else
	{
		/**
		 * 先处理读入buffer_部分的数据 再处理exbuff中的内容
		 */
		ssize_t ex_len = read_size - WritableSize();
		AddWriteIndex(WritableSize());
		CopyExDataToBuffer(exbuffer, ex_len);
	}

	return read_size;
}

void Buffer::CopyExDataToBuffer(const char* exbuffer, size_t ex_len)
{

	ssize_t drop_len = read_idx_ - DEFAULT_READ_INDEX;
	/**
	 * 可以将已读的内容丢弃来 从远buffer_中得到足够的空间
	 * 无法得到则扩容
	 */
	if (drop_len >= ex_len)
	{
		std::copy(ReadBegin(), ReadBegin() + ReadableSize(), &buffer_[DEFAULT_READ_INDEX]);

		write_idx_ -= drop_len;
		read_idx_ = DEFAULT_READ_INDEX;
	}
	else
	{
		buffer_.resize(buffer_.size() + ex_len);
	}

	std::copy(exbuffer, exbuffer + ex_len, WriteBegin());
	AddWriteIndex(ex_len);
}

char* Buffer::ReadBegin()
{
	return &buffer_[read_idx_];
}


char* Buffer::WriteBegin()
{
	return &buffer_[write_idx_];
}

size_t Buffer::ReadableSize() const
{
	return static_cast<size_t>(write_idx_ - read_idx_);
}

size_t Buffer::WritableSize() const
{
	return static_cast<size_t>(buffer_.size() - write_idx_);
}

void Buffer::AddReadIndex(size_t index)
{
	if (index + read_idx_ > buffer_.size())
	{
		read_idx_ = buffer_.size();
	}
	else
	{
		read_idx_ += index;
	}
}

void Buffer::AddWriteIndex(size_t index)
{
	if (index + write_idx_ > buffer_.size())
	{
		write_idx_ = buffer_.size();
	}
	else
	{
		write_idx_ += index;
	}
}

std::string Buffer::ReadAllAsString()
{
	std::string result(ReadBegin(), ReadableSize());
	AddReadIndex(ReadableSize());
	return result;
}

void Buffer::Append(const char* begin, const char* end)
{
	if (end <= begin)
	{
		return;
	}
	Append(begin, end - begin);
}

void Buffer::Append(const char* begin, size_t len)
{
	if (len > WritableSize())
	{
		CopyExDataToBuffer(begin, len);
	}
	else
	{
		std::copy(begin, begin + len, WriteBegin());
		AddWriteIndex(len);
	}
}

void Buffer::Append(const std::string& data)
{
	Append(data.c_str(), data.size());
}

void Buffer::AppendCRLF()
{
	Append(CRLF, CRLF_LEN);
}

void Buffer::Append(Buffer* buffer)
{
	if (!buffer)
	{
		return;
	}
	Append(buffer->ReadBegin(), buffer->ReadableSize());
	buffer->Reset();
}

void Buffer::Reset()
{
	read_idx_ = DEFAULT_READ_INDEX;
	write_idx_ = DEFAULT_WRITE_INDEX;
}

void Buffer::CopyFromBuffer(Buffer* buffer)
{
	if (buffer != nullptr)
	{
		Append(buffer->ReadBegin(), buffer->ReadableSize());
	}
}