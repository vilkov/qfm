#include "filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemItem::FileSystemItem(Item *parent) :
	Item(parent)
{}

bool FileSystemItem::isList() const
{
	return false;
}

FILE_SYSTEM_NS_END
