#include "filesystemwrappednodeitem.h"


FILE_SYSTEM_NS_BEGIN

WrappedNodeItem::WrappedNodeItem() :
	m_container(NULL),
	m_totalSize(0),
	m_removed(true)
{}

WrappedNodeItem::WrappedNodeItem(const IFileContainer *container, IFileInfo::Holder &info, WrappedNodeItem *parent) :
	m_container(container),
	m_totalSize(info->fileSize()),
	m_info(info.take()),
	m_parent(parent),
	m_removed(true)
{
	Q_ASSERT(m_info.data());
}

WrappedNodeItem::~WrappedNodeItem()
{
	qDeleteAll(m_items);
}

WrappedNodeItem::WrappedNodeItem(const IFileContainer *container, WrappedNodeItem *parent) :
	m_container(container),
	m_totalSize(0),
	m_parent(parent),
	m_removed(true)
{}

WrappedNodeItem::WrappedNodeItem(const IFileContainer *container, IFileInfo::size_type totalSize, WrappedNodeItem *parent) :
	m_container(container),
	m_totalSize(totalSize),
	m_parent(parent),
	m_removed(true)
{}

FILE_SYSTEM_NS_END
