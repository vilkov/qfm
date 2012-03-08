#include "filesystemfileinfo.h"


FILE_SYSTEM_NS_BEGIN

FileInfo::FileInfo() :
	isDir(false),
	isFile(false),
	isLink(false),
    exists(false),
	permissions(0),
	size(0)
{}

FILE_SYSTEM_NS_END
