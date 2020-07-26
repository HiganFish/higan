//
// Created by rjd67 on 2020/7/25.
//

#ifndef HIGAN_HTTPRESPONSE_H
#define HIGAN_HTTPRESPONSE_H

#include <string>

#include "higan/http/HttpRequest.h"
#include "higan/Buffer.h"

namespace higan
{
class HttpResponse
{
public:

	enum StatusCode
	{
		STATUS_200_OK = 200,
		STATUS_400_BAD_REQUEST = 400,
		STATUS_404_NOT_FOUND = 404,
		STATUS_500_SERVICE_ERROR = 500
	};

	explicit HttpResponse(bool keep_connection);
	~HttpResponse();

	void SetStatusCode(StatusCode status_code);

	/**
	 * 增加或修改header
	 * @param k 要增加或修改的键
	 * @return 键对应值的引用
	 */
	std::string& operator[](const std::string& k);

	/**
	 * 将Response内容编码到Buffer中
	 * @param buffer
	 */
	void EncodeToBuffer(Buffer* buffer);

	void AppendBody(char* data, size_t len);

	Buffer* GetBodyBuffer();

	static std::string StatusCodeToString(StatusCode status_code);

	bool GetKeepConnection() const;

private:

	bool keep_connection_;

	StatusCode status_code_;

	HttpRequest::HeaderMap header_map_;

	Buffer body_buffer_;
};
}

#endif //HIGAN_HTTPRESPONSE_H
