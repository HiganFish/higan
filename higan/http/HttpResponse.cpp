//
// Created by rjd67 on 2020/7/25.
//

#include "higan/http/HttpResponse.h"
#include "higan/utils/File.h"

using namespace higan;

HttpResponse::HttpResponse(bool close_connection):
	body_buffer_(),
	body_size_(0),
	file_path_(),
	has_file_(false),
	close_connection_(close_connection)
{
	header_map_["Connection"] = close_connection ? "close" : "Keep-Alive";
	header_map_["Content-Length"] = "0";
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

	header_map_["Content-Length"] = std::to_string(body_size_);

	buffer->Append("HTTP/1.1 ");
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

	if (file_path_.empty())
	{
		buffer->Append(&body_buffer_);
	}
}

void HttpResponse::AppendBody(const char* data, size_t len)
{
	if (has_file_)
	{
		return;
	}
	body_buffer_.Append(data, len);
	body_size_ += len;
}

void HttpResponse::AppendBody(const std::string& body)
{
	AppendBody(body.c_str(), body.size());
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


bool HttpResponse::CloseConnection() const
{
	return close_connection_;
}


void HttpResponse::SetFileToResponse(const std::string& file_path)
{
	file_path_ = file_path;
	has_file_ = true;
	/**
	 * TODO 更改文件大小获取方式
	 */
	body_size_ = static_cast<size_t>(File::GetFileSize(file_path_));
}

bool HttpResponse::HasFileToResponse() const
{
	return has_file_;
}

const std::string& HttpResponse::GetFilePath() const
{
	return file_path_;
}
