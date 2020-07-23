//
// Created by rjd67 on 2020/7/22.
//

#ifndef HIGAN_HTTPCONTEXT_H
#define HIGAN_HTTPCONTEXT_H

#include "higan/http/HttpRequest.h"

namespace higan
{
class HttpContext
{
public:

	enum HttpRequestParseStatus
	{
		PARSE_FIRST_LINE,
		PARSE_HEADERS,
		PARSE_ALL
	};

	HttpContext();
	~HttpContext();

	/**
	 * 解析请求
	 * @param begin 首字节指针
	 * @param len 长度
	 * @return 正数为已解析长度 -1代表出错
	 */
	ssize_t ParseRequest(const char* begin, size_t len);

	/**
	 * 重置已解析的request 将解析状态置为解析首行
	 */
	void Reset();

	HttpRequest& GetRequest();
private:

	const static char* CRLF;
	const static size_t CRLF_LEN;


	HttpRequest request_;

	HttpRequestParseStatus status_;

	/**
	 * 解析HTTP请求第一行
	 * @param begin 待解析字符串首字节地址
	 * @param end 待解析字符串尾字节后一位地址
	 * @return 正数为已解析长度 -1出错
	 */
	ssize_t ParseFirstLine(const char* begin, const char* end);

	/**
	 * 解析HTTP请求 header
	 * @param begin 待解析字符串首字节地址
	 * @param end 待解析字符串尾字节后一位地址
	 * @return 正数为已解析长度 -1出错
	 */
	ssize_t ParseHeader(const char* begin, const char* end);
};
}

#endif //HIGAN_HTTPCONTEXT_H
