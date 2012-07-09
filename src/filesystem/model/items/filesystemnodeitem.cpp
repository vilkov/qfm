#include "filesystemnodeitem.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

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

FILE_SYSTEM_NS_END
