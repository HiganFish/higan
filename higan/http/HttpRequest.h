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

	/**
	 * 设置Http请求方法
	 * @param begin 方法首字节地址
	 * @param end 方法尾字节后一位地址
	 */
	bool SetMethod(const char* begin, const char* end);

	/**
	 * 设置HTTP请求url
	 * @param begin url首字节地址
	 * @param end url尾字节后一位地址
	 */
	bool SetUrl(const char* begin, const char* end);

	/**
	 * 设置版本号
	 * @param begin 版本号首字节地址
	 * @param end 版本哈尾字节地址
	 */
	bool SetVersion(const char* begin, const char* end);

	HttpRequestMethod GetMethod() const;
	std::string& GetUrl();
	HttpVersion GetVersion() const;

	/**
	 * 增加一个 kv对 如 DNT: 1
	 * @param begin 单条header的首字节指针
	 * @param end 单条header的尾字节后一位指针 不包括 CRLF
	 */
	bool AddHeader(const char* begin, const char* end);

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
