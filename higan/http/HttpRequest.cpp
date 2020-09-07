//
// Created by rjd67 on 2020/7/22.
//

#include <cstring>
#include <algorithm>

#include "higan/http/HttpRequest.h"
#include "higan/base/Codec.h"

using namespace higan;

const char HttpRequest::COLON[] = ": ";

HttpRequest::HttpRequest():
		method_(HTTP_REQUEST_UNKNOWN),
		url_(),
		version_(HTTP_VERSION_UNKNOWN)
{

}

HttpRequest::~HttpRequest()
{

}

bool HttpRequest::SetMethod(const char* begin, const char* end)
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
	else
	{
		return false;
	}
	return true;
}

bool HttpRequest::SetUrl(const char* begin, const char* end)
{
	if (end - begin < 1)
	{
		return false;
	}
	url_ = Codec::DecodeUrl(begin, end - begin);

	return true;
}

bool HttpRequest::SetVersion(const char* begin, const char* end)
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
	else
	{
		return false;
	}

	return true;
}

bool HttpRequest::AddHeader(const char* begin, const char* end)
{

	const char* k_end = std::search(begin, end, COLON, COLON + 2);

	if (end - k_end < 3)
	{
		return false;
	}

	headers_[{ begin, k_end}] = { k_end + 2, end};
	return true;
}

void HttpRequest::AppendBody(const char* begin, const char* end)
{
	body_.Append(begin, end);
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

std::string HttpRequest::operator[](const std::string& key) const
{
	auto result = headers_.find(key);
	if (result != headers_.end())
	{
		return result->second;
	}
	return "";
}

std::string HttpRequest::GetMethodString() const
{
	std::string result = "ERROR";
	switch (method_)
	{
		case HTTP_REQUEST_UNKNOWN:
			result = "UNKNOWN";
			break;
		case HTTP_REQUEST_GET:
			result = "GET";
			break;
		case HTTP_REQUEST_POST:
			result = "POST";
			break;
		case HTTP_REQUEST_PUT:
			result = "PUT";
			break;
		case HTTP_REQUEST_DELETE:
			result = "DELETE";
			break;
	}

	return result;
}

const std::string& HttpRequest::GetUrl() const
{
	return url_;
}

const HttpRequest::HeaderMap& HttpRequest::GetHeaderMap() const
{
	return headers_;
}
