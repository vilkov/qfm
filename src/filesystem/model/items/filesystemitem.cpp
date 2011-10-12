#include "filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemItem::FileSystemItem(Item *parent) :
	Item(parent),
	m_locked(false)
{}

void FileSystemItem::lock(const QString &reason)
{
	m_locked = true;
	m_lockReason = reason;
}

void FileSystemItem::unlock()
{
	m_locked = false;
	m_lockReason.clear();
}

FILE_SYSTEM_NS_END
