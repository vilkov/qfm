#include "filesystemtasknodeitem.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

TaskNodeItem::TaskNodeItem(Base *parent) :
	FileSystemItem(parent),
	m_locked(false),
	m_total(0),
	m_timeElapsed(0)
{}

void TaskNodeItem::lock(const QString &reason)
{
	m_reason = reason;
	m_icon = Application::desktopEnvironment()->processingIcon(16);
	m_locked = true;
}

FILE_SYSTEM_NS_END
