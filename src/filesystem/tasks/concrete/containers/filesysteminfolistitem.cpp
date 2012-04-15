#include "filesysteminfolistitem.h"


FILE_SYSTEM_NS_BEGIN

InfoListItem::InfoListItem(const IFileContainer *container, const Info &file) :
	InfoItem(container, file),
	m_container(container->location(file.fileName())),
	m_totalSize(0)
{}

InfoListItem::InfoListItem(const IFileContainer *container, const QString &file) :
	InfoItem(container, file),
	m_container(container->location(file)),
	m_totalSize(0)
{}

InfoListItem::~InfoListItem()
{
	qDeleteAll(m_items);
}

Info::size_type InfoListItem::totalSize() const
{
	return m_totalSize;
}

FILE_SYSTEM_NS_END
