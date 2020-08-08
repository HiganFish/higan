//
// Created by rjd67 on 2020/7/30.
//

#ifndef HIGAN_FILECONTEXT_H
#define HIGAN_FILECONTEXT_H

#include <memory>
#include "higan/base/File.h"

namespace higan
{
class FileContext
{
public:
	typedef std::shared_ptr<FileContext> FileContextPtr;

	FileContext(const FileForRead::FileForReadPtr & file_ptr, bool close_connection);
	~FileContext();

	ssize_t ReadFileToBuffer(Buffer* buffer);

	bool CloseConnection() const;

private:

	bool close_connection_;


	size_t sum_read_from_cache_;
	bool read_over_;

	FileForRead::FileForReadPtr file_ptr_;
};
}

#endif //HIGAN_FILECONTEXT_H
