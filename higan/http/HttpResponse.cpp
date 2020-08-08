//
// Created by rjd67 on 2020/7/25.
//

#include "higan/http/HttpResponse.h"


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

	char line_buff[64];
	int len = snprintf(line_buff, sizeof line_buff, "HTTP/1.1 %s\r\n", StatusCodeToString(status_code_).c_str());
	buffer->Append(line_buff, len);

	if (close_connection_)
	{
		buffer->Append("Connection: close\r\n");
	}
	else
	{
		buffer->Append("Connection: Keep-Alive\r\n");
	}

	size_t body_size = body_buffer_.ReadableSize();
	if (file_ptr_)
	{
		body_size += file_ptr_->GetFileSize();
	}

	len = snprintf(line_buff, sizeof line_buff, "Content-Length: %zd\r\n", body_size);
	buffer->Append(line_buff, len);


	for (const auto& kv : header_map_)
	{
		buffer->Append(kv.first);
		buffer->Append(": ");
		buffer->Append(kv.second);
		buffer->AppendCRLF();
	}

	buffer->AppendCRLF();

	buffer->Append(&body_buffer_);
	if (file_ptr_)
	{
		buffer->Append(&body_buffer_);
	}
}

void HttpResponse::AppendBody(const char* data, size_t len)
{
	if (file_ptr_)
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
	file_ptr_ = std::make_shared<FileForRead>(file_path);

	bool add_ok_result = false;

	switch (file_ptr_->GetFileStatus())
	{
	case FileForRead::FileStatus::NOT_EXIST:
		add_ok_result = false;
		break;
	case FileForRead::FileStatus::IS_DIR:
		add_ok_result = SetFileToResponse(file_path + "/index.html");
		break;
	case FileForRead::FileStatus::OPEN_SUCCESS:
		add_ok_result = true;
		break;
	case FileForRead::FileStatus::OPEN_ERROR:
		add_ok_result = false;
		break;
	}

	if (!add_ok_result)
	{
		file_ptr_.reset();
	}
	return add_ok_result;
}

bool HttpResponse::HasFileToResponse() const
{
	return file_ptr_ != nullptr;
}

const FileForRead::FileForReadPtr & HttpResponse::GetFilePtr() const
{
	return file_ptr_;
}

void HttpResponse::SetCloseConnection()
{
	close_connection_ = true;
}
