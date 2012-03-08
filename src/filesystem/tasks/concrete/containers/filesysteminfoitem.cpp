#include "filesysteminfoitem.h"


FILE_SYSTEM_NS_BEGIN

InfoItem::InfoItem(const QString &filePath) :
	Info(filePath, Identify()),
	m_removed(true)
{}

Info::size_type InfoItem::totalSize() const
{
	return fileSize();
}

FILE_SYSTEM_NS_END
