#include "filesysteminfoitem.h"


FILE_SYSTEM_NS_BEGIN

InfoItem::InfoItem(const Info &info) :
	Info(info),
	m_removed(true)
{}

InfoItem::InfoItem(const QString &filePath) :
	Info(filePath),
	m_removed(true)
{}

InfoItem::InfoItem(const QString &filePath, bool identifyType) :
	Info(filePath, identifyType),
	m_removed(true)
{}

Info::size_type InfoItem::totalSize() const
{
	return fileSize();
}

FILE_SYSTEM_NS_END
