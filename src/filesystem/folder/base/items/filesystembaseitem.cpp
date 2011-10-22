#include "filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemBaseItem::FileSystemBaseItem(const Info &info, Item *parent) :
	FileSystemItem(parent)
{}

bool FileSystemBaseItem::isDir() const
{
	return m_info.isDir();
}

bool FileSystemBaseItem::isFile() const
{
	return m_info.isFile();
}

bool FileSystemBaseItem::exists() const
{
	return m_info.exists();
}

QString FileSystemBaseItem::fileName() const
{
	return m_info.isRoot() ? m_info.absoluteFilePath() : m_info.fileName();
}

QString FileSystemBaseItem::absolutePath() const
{
	return m_info.absolutePath();
}

QString FileSystemBaseItem::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString FileSystemBaseItem::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime FileSystemBaseItem::lastModified() const
{
	return m_info.lastModified();
}

void FileSystemBaseItem::refresh()
{
	return m_info.refresh();
}

FILE_SYSTEM_NS_END
