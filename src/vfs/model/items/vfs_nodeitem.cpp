#include "vfs_nodeitem.h"
#include "../../../application.h"


VFS_NS_BEGIN

NodeItem::NodeItem(Base *parent) :
	Base(parent),
	m_locked(false)
{}

void NodeItem::lock(const QString &reason)
{
	m_reason = reason;
	m_icon = Application::desktopService()->processingIcon();
	m_locked = true;
}

void NodeItem::cancel(const QString &reason)
{
	m_reason = reason;
	m_icon = Application::desktopService()->cancelingIcon();
}

VFS_NS_END
