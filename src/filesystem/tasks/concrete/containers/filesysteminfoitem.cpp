#include "filesysteminfoitem.h"


FILE_SYSTEM_NS_BEGIN

InfoItem::InfoItem(const IFileContainer *container, const QString &file) :
	Info(container->location(file), Identify()),
	m_container(container),
	m_removed(true)
{}

Info::size_type InfoItem::totalSize() const
{
	return fileSize();
}

FILE_SYSTEM_NS_END
