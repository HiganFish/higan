//
// Created by rjd67 on 2020/7/22.
//

#include <algorithm>
#include <cstring>

#include "higan/base/System.h"
#include "higan/http/HttpContext.h"

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

bool HttpContext::ParseRequest(Buffer* buffer)
{
	bool result_ok = true;

	while (true)
	{
		const char* data = buffer->ReadBegin();
		size_t len = buffer->ReadableSize();

		if (status_ == PARSE_FIRST_LINE)
		{
			ssize_t p_len = ParseFirstLine(data, data + len);
			if (p_len == -1)
			{
				result_ok = false;
				break;
			}
			buffer->AddReadIndex(p_len);
		}
		else if (status_ == PARSE_HEADERS)
		{
			ssize_t p_len = ParseHeader(data, data + len);
			if (p_len == -1)
			{
				result_ok = false;
				break;;
			}
			buffer->AddReadIndex(p_len);
		}
		else if (status_ == PARSE_ALL)
		{
			request_.AppendBody(data, data + len);
			buffer->Reset();
			break;
		}
	}
	return result_ok;
}

ssize_t HttpContext::ParseFirstLine(const char* begin, const char* end)
{
	// GET /hello.html HTTP/1.1\r\n

	const char* first_line_end = std::search(begin, end, CRLF, CRLF + CRLF_LEN);
	CHECK_IF_RETURN(first_line_end == end, true, 0);

	// GET
	const char* method_end = std::find(begin, first_line_end, ' ');
	CHECK_IF_RETURN(method_end == first_line_end, true, -1);
	CHECK_IF_RETURN(request_.SetMethod(begin, method_end), false, -1);


	// /hello.html
	const char* url_end = std::find(method_end + 1, first_line_end, ' ');
	CHECK_IF_RETURN(url_end == first_line_end, true, -1);
	CHECK_IF_RETURN(request_.SetUrl(method_end + 1, url_end), false, -1);


	CHECK_IF_RETURN(first_line_end - url_end != 9, true, -1);
	// HTTP/1.1
	CHECK_IF_RETURN(request_.SetVersion(url_end + 1, first_line_end), false, -1);


	status_ = PARSE_HEADERS;
	return first_line_end - begin + CRLF_LEN;
}

ssize_t HttpContext::ParseHeader(const char* begin, const char* end)
{
	const char* line_begin = begin;
	const char* line_end = end;

	while ((line_end = std::search(line_begin, end, CRLF, CRLF + CRLF_LEN)) != end)
	{
		/**
		 * xxxx: xxxCRLFCRLF
		 * 搜索到两个连续的CRLF代表首部结束
		 */
		if (line_begin == line_end)
		{
			status_ = PARSE_ALL;
			break;
		}

		CHECK_IF_RETURN(request_.AddHeader(line_begin, line_end), false, -1);

		line_begin = line_end + CRLF_LEN;
	}

	return line_end + CRLF_LEN - begin;
}

void HttpContext::Reset()
{
	status_ = PARSE_FIRST_LINE;
	request_ = HttpRequest();
}

HttpRequest& HttpContext::GetRequest()
{
	return request_;
}

bool HttpContext::ParseOver() const
{
	return status_ == PARSE_ALL;
}
