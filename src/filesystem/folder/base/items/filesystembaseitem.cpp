#include "filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemBaseItem::FileSystemBaseItem(const Info &info, Item *parent) :
	FileSystemItem(parent),
	Info(info)
{}

QString FileSystemBaseItem::fileName() const
{
	return isRoot() ? absoluteFilePath() : Info::fileName();
}

FILE_SYSTEM_NS_END
