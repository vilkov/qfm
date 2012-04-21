#include "filesysteminfoitem.h"


FILE_SYSTEM_NS_BEGIN

InfoItem::InfoItem(const IFileContainer *container, const Info &file) :
	Info(file),
	m_container(container),
	m_removed(true)
{}

InfoItem::InfoItem(const IFileContainer *container, const QString &file) :
	Info(container->location(file), Info::Identify()),
	m_container(container),
	m_removed(true)
{}

Info::size_type InfoItem::totalSize() const
{
	return fileSize();
}

FILE_SYSTEM_NS_END
