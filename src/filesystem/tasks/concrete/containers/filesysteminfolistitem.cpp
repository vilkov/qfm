#include "filesysteminfolistitem.h"


FILE_SYSTEM_NS_BEGIN

InfoListItem::InfoListItem(const IFileContainer *container, const Info &file) :
	InfoItem(container, file),
	m_totalSize(0),
	m_container(container->open(file.fileName(), false, m_lastError))
{}

InfoListItem::InfoListItem(const IFileContainer *container, const QString &file) :
	InfoItem(container, file),
	m_totalSize(0),
	m_container(container->open(file, false, m_lastError))
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
