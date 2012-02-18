#include "filesystemitem.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FileSystemItem::FileSystemItem(Base *parent) :
	Base(parent),
	m_locked(false)
{}

void FileSystemItem::lock(const QString &reason)
{
	m_reason = reason;
	m_icon = Application::desktopService()->processingIcon(16);
	m_locked = true;
}

void FileSystemItem::cancel(const QString &reason)
{
	m_reason = reason;
	m_icon = Application::desktopService()->cancelingIcon(16);
}

FILE_SYSTEM_NS_END
