//
// Created by rjd67 on 2020/7/22.
//

#include <algorithm>
#include <cstring>

#include "HttpContext.h"

using namespace higan;

const char* HttpContext::CRLF = "\r\n";
const size_t HttpContext::CRLF_LEN = strlen(CRLF);


HttpContext::HttpContext():
		status_(PARSE_FIRST_LINE)
{

}

HttpContext::~HttpContext()
{

}

ssize_t HttpContext::ParseRequest(const char* begin, size_t len)
{
	size_t parsed_len = 0;

	if (status_ == PARSE_FIRST_LINE)
	{
		ssize_t p_len = ParseFirstLine(begin, begin + len);
		if (p_len == -1)
		{
			return -1;
		}
		parsed_len += p_len;
	}

	if (status_ == PARSE_HEADERS)
	{
		ssize_t p_len = ParseHeader(begin + parsed_len, begin + len);
		if (p_len == -1)
		{
			return -1;
		}
		parsed_len += p_len;
	}

	if (status_ == PARSE_ALL)
	{
		request_.AppendBody(begin + parsed_len, begin + len);
		return len;
	}

	return parsed_len;
}

ssize_t HttpContext::ParseFirstLine(const char* begin, const char* end)
{
	const char* first_line_end = std::search(begin, end, CRLF, CRLF + CRLF_LEN);
	if (first_line_end == end)
	{
		return 0;
	}

	const char* method_end = std::find(begin, first_line_end, ' ');
	if (method_end == first_line_end)
	{
		return -1;
	}
	request_.SetMethod(begin, method_end);

	const char* url_end = std::find(method_end + 1, first_line_end, ' ');
	if (url_end == first_line_end)
	{
		return -1;
	}
	request_.SetUrl(method_end + 1, url_end);

	if (first_line_end - url_end != 9)
	{
		return -1;
	}
	request_.SetVersion(url_end + 1, first_line_end);

	status_ = PARSE_HEADERS;

	return first_line_end - begin + CRLF_LEN;
}

ssize_t HttpContext::ParseHeader(const char* begin, const char* end)
{
	const char* line_begin = begin;
	const char* line_end = end;

	while ((line_end = std::search(line_begin, end, CRLF, CRLF + CRLF_LEN)) != end)
	{
		if (line_begin == line_end)
		{
			status_ = PARSE_ALL;
			return line_end + CRLF_LEN - begin;
		}

		request_.AddHeader(line_begin, line_end);
		line_begin = line_end + CRLF_LEN;
	}

	return line_end + CRLF_LEN - begin;
}
