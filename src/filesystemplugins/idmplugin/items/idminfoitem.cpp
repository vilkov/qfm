#include "idminfoitem.h"


FILE_SYSTEM_NS_BEGIN

IdmInfoItem::IdmInfoItem(const Info &info, IdmItem *parent) :
	IdmItem(parent),
	m_info(info)
{}

bool IdmInfoItem::isDir() const
{
	return m_info.isDir();
}

bool IdmInfoItem::isFile() const
{
	return m_info.isFile();
}

bool IdmInfoItem::exists() const
{
	return m_info.exists();
}

QString IdmInfoItem::fileName() const
{
	return m_info.fileName();
}

QString IdmInfoItem::absolutePath() const
{
	return m_info.absolutePath();
}

QString IdmInfoItem::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString IdmInfoItem::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime IdmInfoItem::lastModified() const
{
	return m_info.lastModified();
}

void IdmInfoItem::refresh()
{
	m_info.refresh();
}

bool IdmInfoItem::isRoot() const
{
	return false;
}

bool IdmInfoItem::isList() const
{
	return false;
}

bool IdmInfoItem::isMenuItem() const
{
	return false;
}

bool IdmInfoItem::isEntityItem() const
{
	return false;
}

FILE_SYSTEM_NS_END
