#include "vfs_snapshotitem.h"


VFS_NS_BEGIN

SnapshotItem::SnapshotItem() :
	m_container(NULL),
	m_totalSize(0),
	m_removed(true)
{}

SnapshotItem::SnapshotItem(const IFileContainer *container, IFileInfo::Holder &info, SnapshotItem *parent) :
	m_container(container),
	m_totalSize(info->fileSize()),
	m_info(info.take()),
	m_parent(parent),
	m_removed(true)
{
	Q_ASSERT(m_info.data());
}

SnapshotItem::~SnapshotItem()
{
	qDeleteAll(m_items);
}

SnapshotItem::SnapshotItem(const IFileContainer *container, SnapshotItem *parent) :
	m_container(container),
	m_totalSize(0),
	m_parent(parent),
	m_removed(true)
{}

SnapshotItem::SnapshotItem(const IFileContainer *container, IFileInfo::size_type totalSize, SnapshotItem *parent) :
	m_container(container),
	m_totalSize(totalSize),
	m_parent(parent),
	m_removed(true)
{}

VFS_NS_END
