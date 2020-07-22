//
// Created by rjd67 on 2020/7/22.
//

#include <cstring>
#include <algorithm>

#include "higan/http/HttpRequest.h"

using namespace higan;

const char* HttpRequest::KV_SEPARATOR = ": ";
const size_t HttpRequest::KV_SEPARATOR_LEN = strlen(KV_SEPARATOR);

HttpRequest::HttpRequest():
		method_(HTTP_REQUEST_UNKNOWN),
		url_(),
		version_(HTTP_VERSION_UNKNOWN)
{

}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::SetMethod(const char* begin, const char* end)
{
	std::string method(begin, end);

	if (method == "GET")
	{
		method_ = HTTP_REQUEST_GET;
	}
	else if (method == "POST")
	{
		method_ = HTTP_REQUEST_POST;
	}
	else if (method == "PUT")
	{
		method_ = HTTP_REQUEST_PUT;
	}
	else if (method == "DELETE")
	{
		method_ = HTTP_REQUEST_DELETE;
	}
}

void HttpRequest::SetUrl(const char* begin, const char* end)
{
	url_.append(begin, end);
}

void HttpRequest::SetVersion(const char* begin, const char* end)
{
	std::string version(begin, end);

	if (version == "HTTP/1.0")
	{
		version_ = HTTP_VERSION_10;
	}
	else if (version == "HTTP/1.1")
	{
		version_ = HTTP_VERSION_11;
	}
}

void HttpRequest::AddHeader(const char* begin, const char* end)
{
	const char* k_end = std::search(begin, end, KV_SEPARATOR, KV_SEPARATOR + KV_SEPARATOR_LEN);

	if (end - k_end < 3)
	{
		return;
	}

	headers_[{ begin, k_end}] = { k_end + 2, end};
}

void HttpRequest::AppendBody(const char* begin, const char* end)
{

}

HttpRequest::HttpRequestMethod HttpRequest::GetMethod() const
{
	return method_;
}

std::string& HttpRequest::GetUrl()
{
	return url_;
}

HttpRequest::HttpVersion HttpRequest::GetVersion() const
{
	return version_;
}
