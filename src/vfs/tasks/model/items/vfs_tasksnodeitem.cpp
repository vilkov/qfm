#include "vfs_tasksnodeitem.h"


VFS_NS_BEGIN

TasksNodeItem::TasksNodeItem(Base *parent) :
	NodeItem(parent),
	m_total(0),
	m_timeElapsed(0)
{}

VFS_NS_END
