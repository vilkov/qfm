#include "filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemBaseItem::FileSystemBaseItem(const Info &info, Base *parent) :
	TaskNodeItem(parent),
	m_node(0),
	m_info(info)
{}

bool FileSystemBaseItem::isList() const
{
	return false;
}

FileSystemBaseItem::ItemInfo::ItemInfo(const Info &info) :
	Info(info)
{}

QString FileSystemBaseItem::ItemInfo::fileName() const
{
	return isRoot() ? absoluteFilePath() : Info::fileName();
}

FILE_SYSTEM_NS_END
