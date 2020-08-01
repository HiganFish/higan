//
// Created by rjd67 on 2020/7/25.
//

#include "higan/http/HttpResponse.h"
#include "higan/utils/File.h"

using namespace higan;

HttpResponse::HttpResponse(bool close_connection):
	body_buffer_(),
	close_connection_(close_connection),
	file_ptr_()
{
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

	if (file_ptr_)
	{
		header_map_["Content-Length"] = std::to_string(file_ptr_->GetFileSize());
	}
	else
	{
		header_map_["Content-Length"] = std::to_string(body_buffer_.ReadableSize());
	}


	buffer->Append("HTTP/1.1 ");
	buffer->Append(StatusCodeToString(status_code_));
	buffer->AppendCRLF();

	header_map_["Connection"] = close_connection_ ? "close" : "Keep-Alive";

	for (const auto& kv : header_map_)
	{
		std::string line = kv.first;
		line.append(HttpRequest::KV_SEPARATOR, HttpRequest::KV_SEPARATOR_LEN);
		line.append(kv.second);

		buffer->Append(line);
		buffer->AppendCRLF();
	}

	buffer->AppendCRLF();

	if (!file_ptr_)
	{
		buffer->Append(&body_buffer_);
	}
}

void HttpResponse::AppendBody(const char* data, size_t len)
{
	if (!file_ptr_)
	{
		return;
	}
	body_buffer_.Append(data, len);
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


bool HttpResponse::SetFileToResponse(const std::string& file_path)
{
	file_ptr_ = std::make_shared<File>(file_path);
	bool add_result = true;

	switch (file_ptr_->GetFileStatus())
	{
	case File::FileStatus::NOT_EXIST:
		add_result = false;
		break;
	case File::FileStatus::IS_DIR:
		add_result = SetFileToResponse(file_path + "/index.html");
		break;
	case File::FileStatus::FILE_OPEN_SUCCESS:
		add_result = true;
		break;
	case File::FileStatus::FILE_OPEN_ERROR:
		add_result = false;
		break;
	}

	if (!add_result)
	{
		file_ptr_.reset();
	}
	return add_result;
}

bool HttpResponse::HasFileToResponse() const
{
	return file_ptr_ != nullptr;
}

const File::FilePtr& HttpResponse::GetFilePtr() const
{
	return file_ptr_;
}
