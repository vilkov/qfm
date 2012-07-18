#include "vfs_item.h"
#include "../../../application.h"


VFS_NS_BEGIN

Item::Item(Base *parent) :
	Base(parent),
	m_locked(false)
{}

void Item::lock(const QString &reason)
{
	m_reason = reason;
	m_icon = Application::desktopService()->processingIcon();
	m_locked = true;
}

void Item::cancel(const QString &reason)
{
	m_reason = reason;
	m_icon = Application::desktopService()->cancelingIcon();
}

VFS_NS_END
