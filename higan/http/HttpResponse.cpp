//
// Created by rjd67 on 2020/7/25.
//

#include "HttpResponse.h"

using namespace higan;

HttpResponse::HttpResponse(bool keep_connection)
{
	header_map_["Connection"] = keep_connection ? "keep-alive" : "close";

}

HttpResponse::~HttpResponse()
{

}

void HttpResponse::SetStatusCode(HttpResponse::StatusCode status_code)
{
	status_code_ = status_code;
}

std::string& HttpResponse::operator[](const std::string& k)
{
	return header_map_[k];
}

void HttpResponse::EncodeToBuffer(Buffer* buffer)
{
	if (!buffer)
	{
		return;
	}

	header_map_["Content-Length"] = std::to_string(body_buffer_.ReadableSize());

	buffer->Append("HTTP/1.1 ", 9);
	buffer->Append(StatusCodeToString(status_code_));
	buffer->AppendCRLF();

	for (const auto& kv : header_map_)
	{
		std::string line = kv.first;
		line.append(HttpRequest::KV_SEPARATOR, HttpRequest::KV_SEPARATOR_LEN);
		line.append(kv.second);

		buffer->Append(line);
		buffer->AppendCRLF();
	}

	buffer->AppendCRLF();
	buffer->Append(&body_buffer_);
}

void HttpResponse::AppendBody(char* data, size_t len)
{
	body_buffer_.Append(data, len);
}

std::string HttpResponse::StatusCodeToString(HttpResponse::StatusCode status_code)
{
	std::string result = "500 Internal Server Error";
	switch (status_code)
	{
	case STATUS_200_OK:
		result = "200 OK";
		break;
	case STATUS_400_BAD_REQUEST:
		result = "400 Bad Request";
		break;
	case STATUS_404_NOT_FOUND:
		result = "404 Not Found";
		break;
	case STATUS_500_SERVICE_ERROR:
		result = "500 Internal Server Error";
		break;
	}

	return result;
}


bool HttpResponse::GetKeepConnection() const
{
	return keep_connection_;
}

Buffer* HttpResponse::GetBodyBuffer()
{
	return &body_buffer_;
}
