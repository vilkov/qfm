#include "filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemBaseItem::FileSystemBaseItem(const Info &info, Item *parent) :
	FileSystemItem(parent),
	m_info(info)
{}

FileSystemBaseItem::ItemInfo::ItemInfo(const Info &info) :
	Info(info)
{}

QString FileSystemBaseItem::ItemInfo::fileName() const
{
	return isRoot() ? absoluteFilePath() : Info::fileName();
}

FILE_SYSTEM_NS_END
