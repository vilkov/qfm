#include "filesysteminfoentryitem.h"


FILE_SYSTEM_NS_BEGIN

InfoEntryItem::InfoEntryItem(const Info &info) :
	InfoItem(info)
{}

bool InfoEntryItem::isList() const
{
	return false;
}

qint64 InfoEntryItem::totalSize() const
{
	return fileSize();
}

FILE_SYSTEM_NS_END
