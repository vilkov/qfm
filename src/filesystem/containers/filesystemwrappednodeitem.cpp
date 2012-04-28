#include "filesystemwrappednodeitem.h"


FILE_SYSTEM_NS_BEGIN

WrappedNodeItem::WrappedNodeItem() :
	m_container(NULL),
	m_totalSize(0),
	m_removed(false)
{}

WrappedNodeItem::WrappedNodeItem(const IFileContainer *container, IFileInfo *info) :
	m_container(container),
	m_totalSize(0),
	m_info(info),
	m_removed(false)
{
	Q_ASSERT(m_info.data());
}

WrappedNodeItem::WrappedNodeItem(const IFileContainer *container, IFileInfo::Holder &info) :
	m_container(container),
	m_totalSize(0),
	m_info(info.take()),
	m_removed(false)
{
	Q_ASSERT(m_info.data());
}

WrappedNodeItem::~WrappedNodeItem()
{
	qDeleteAll(m_items);
}

FILE_SYSTEM_NS_END
