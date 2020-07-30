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

	FileContext(const std::string& url, bool keep_connection);
	~FileContext();

	ssize_t ReadFileToBuffer(Buffer* buffer);

	bool IsKeepConnection() const;

private:
	std::string url_;

	bool keep_connection_;

	File file_;
};
}

#endif //HIGAN_FILECONTEXT_H
