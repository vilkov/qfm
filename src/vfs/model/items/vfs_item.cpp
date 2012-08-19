#include "vfs_item.h"
#include "../../../desktop/theme/desktop_theme.h"


VFS_NS_BEGIN

Item::Item(Base *parent) :
	Base(parent),
	m_locked(false)
{}

void Item::lock(const QString &reason)
{
	m_reason = reason;
	m_icon = ::Desktop::Theme::current()->processingIcon();
	m_locked = true;
}

void Item::cancel(const QString &reason)
{
	m_reason = reason;
	m_icon = ::Desktop::Theme::current()->cancelingIcon();
}

VFS_NS_END
