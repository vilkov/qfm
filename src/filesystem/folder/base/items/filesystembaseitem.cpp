#include "filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemBaseItem::FileSystemBaseItem(const Info &info, const Node::Holder &node, Item *parent) :
	FileSystemItem(node, parent),
	Info(info)
{}

QString FileSystemBaseItem::fileName() const
{
	return isRoot() ? absoluteFilePath() : Info::fileName();
}

FILE_SYSTEM_NS_END
