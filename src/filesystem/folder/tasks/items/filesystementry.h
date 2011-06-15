#ifndef FILESYSTEMENTRY_H_
#define FILESYSTEMENTRY_H_

#include "filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemEntry : public FileSystemItem
{
public:
	explicit FileSystemEntry(const Info &info) :
		FileSystemItem(info)
	{}

	virtual bool isList() const { return false; }
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMENTRY_H_ */
