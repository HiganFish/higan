//
// Created by rjd67 on 2020/7/29.
//

#include <algorithm>
#include <cmath>
#include "Codec.h"

using namespace higan;

Codec::Codec()
{

}

Codec::~Codec()
{

}

std::string Codec::DecodeUrl(const std::string& url)
{
	return DecodeUrl(url.c_str(), url.length());
}

std::string Codec::DecodeUrl(const char* url, size_t len)
{
	char* buffer = new char[len + 1]{};

	const char* url_ptr = url;
	const char* url_end = url + len;

	char* buffer_ptr = buffer;

	while (url_ptr < url_end)
	{
		if (*url_ptr == '%')
		{
			url_ptr++;
			/**
			 * % 后面需要有两位字符 不足两位说明存在问题 拷贝剩余字符后break
			 */
			if (url_end - url_ptr >= 2)
			{
				int num = HexCharArrayToInt(url_ptr, url_ptr + 2);
				if (num == -1)
				{
					return "";
				}
				*buffer_ptr++ = static_cast<char>(num);
				url_ptr += 2;
			}
			else
			{
				std::copy(url_ptr, url_end, buffer_ptr);
				break;
			}
		}
		else
		{
			*buffer_ptr++ = *url_ptr++;
		}
	}

	return std::string(buffer, buffer_ptr);
}



int Codec::HexCharToInt(char character)
{
	int result = -1;
	if (character >= 'a' && character <= 'f')
	{
		result = character - 'a' + 10;
	}
	else if (character >= 'A' && character <= 'F')
	{
		result = character - 'A' + 10;
	}
	else if (character >= '0' && character <= '9')
	{
		result = character - '0';
	}
	return result;
}

int Codec::HexCharArrayToInt(const char* begin, const char* end)
{
	if (end <= begin)
	{
		return -1;
	}

	int index = end - begin - 1;

	int result = 0;

	std::for_each(begin, end, [&](char i)
	{
		result += std::pow(16, index--) * HexCharToInt(i);
	});

	return result < 0 ? -1 : result;
}
