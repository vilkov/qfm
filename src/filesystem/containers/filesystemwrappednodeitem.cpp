#include "filesystemwrappednodeitem.h"


FILE_SYSTEM_NS_BEGIN

WrappedNodeItem::WrappedNodeItem(const IFileContainer *container, NodeItem *item) :
	m_container(container),
	m_totalSize(0),
	m_item(item),
	m_removed(false)
{}

WrappedNodeItem::~WrappedNodeItem()
{
	qDeleteAll(m_items);
	delete m_item;
}

FILE_SYSTEM_NS_END
