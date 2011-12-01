#include "filesysteminfolistitem.h"


FILE_SYSTEM_NS_BEGIN

InfoListItem::InfoListItem(const Info &info) :
	InfoItem(info),
	m_totalSize(0)
{}

#ifndef Q_OS_WIN
InfoListItem::InfoListItem(const Info &info, uint userId, uint groupId) :
	InfoItem(info),
	m_totalSize(0)
{}
#endif

InfoListItem::~InfoListItem()
{
	qDeleteAll(m_items);
}

bool InfoListItem::isList() const
{
	return true;
}

qint64 InfoListItem::totalSize() const
{
	return m_totalSize;
}

FILE_SYSTEM_NS_END
