//
// Created by rjd67 on 2020/7/22.
//

#ifndef HIGAN_HTTPREQUEST_H
#define HIGAN_HTTPREQUEST_H

#include <string>
#include <map>

#include "higan/Buffer.h"

namespace higan
{
class HttpRequest
{
public:
	enum HttpRequestMethod
	{
		HTTP_REQUEST_UNKNOWN,
		HTTP_REQUEST_GET,
		HTTP_REQUEST_POST,
		HTTP_REQUEST_PUT,
		HTTP_REQUEST_DELETE
	};

	enum HttpVersion
	{
		HTTP_VERSION_UNKNOWN,
		HTTP_VERSION_10,
		HTTP_VERSION_11
	};

	HttpRequest();
	~HttpRequest();

	void SetMethod(const char* begin, const char* end);
	void SetUrl(const char* begin, const char* end);
	void SetVersion(const char* begin, const char* end);

	HttpRequestMethod GetMethod() const;
	std::string& GetUrl();
	HttpVersion GetVersion() const;

	/**
	 * 增加一个 kv对 如 DNT: 1
	 * @param begin 单条header的头字符指针
	 * @param end 单条header的尾字符后一位指针
	 */
	void AddHeader(const char* begin, const char* end);

	void AppendBody(const char* begin, const char* end);
private:

	const static char* KV_SEPARATOR;
	const static size_t KV_SEPARATOR_LEN;

	HttpRequestMethod method_;
	std::string url_;
	HttpVersion version_;

	std::map<std::string, std::string> headers_;

	Buffer body_;
};

}

#endif //HIGAN_HTTPREQUEST_H
