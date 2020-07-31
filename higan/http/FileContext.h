//
// Created by rjd67 on 2020/7/30.
//

#ifndef HIGAN_FILECONTEXT_H
#define HIGAN_FILECONTEXT_H

#include <memory>
#include "higan/utils/File.h"

namespace higan
{
class FileContext
{
public:
	typedef std::shared_ptr<FileContext> FileContextPtr;

	FileContext(const std::string& url, bool close_connection);
	~FileContext();

	ssize_t ReadFileToBuffer(Buffer* buffer);

	bool CloseConnection() const;

private:
	std::string url_;

	bool close_connection_;

	File file_;
};
}

#endif //HIGAN_FILECONTEXT_H
