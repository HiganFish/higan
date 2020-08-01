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

	FileContext(const File::FilePtr& file_ptr, bool close_connection);
	~FileContext();

	ssize_t ReadFileToBuffer(Buffer* buffer);

	bool CloseConnection() const;

private:

	bool close_connection_;

	File::FilePtr file_ptr_;
};
}

#endif //HIGAN_FILECONTEXT_H
