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

	ssize_t ParseRequest(const char* begin, size_t len);
private:

	const static char* CRLF;
	const static size_t CRLF_LEN;


	HttpRequest request_;

	HttpRequestParseStatus status_;

	ssize_t ParseFirstLine(const char* begin, const char* end);
	ssize_t ParseHeader(const char* begin, const char* end);
};
}

#endif //HIGAN_HTTPCONTEXT_H
