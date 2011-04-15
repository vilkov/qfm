#include "filesysteminfo.h"
#include <QFSFileEngine>
#include "../../../application.h"


FileSystemInfo::FileSystemInfo() :
	QFileInfo()
{}

FileSystemInfo::FileSystemInfo(const QFileInfo &info) :
	QFileInfo(info)
{}

bool FileSystemInfo::operator ==(const FileSystemInfo &fileInfo) const
{
   return QFileInfo::operator==(fileInfo) &&
   m_displayType == fileInfo.m_displayType &&
   permissions() == fileInfo.permissions();
}

void FileSystemInfo::refresh()
{
	m_displayType = QString();
	m_icon = QIcon();
	QFileInfo::refresh();
}

const QIcon &FileSystemInfo::icon() const
{
	if (m_icon.isNull())
		m_icon = Application::instance()->iconProvider().icon(*this);

	return m_icon;
}

const QString &FileSystemInfo::displayType() const
{
	if (m_displayType.isEmpty())
		m_displayType = Application::instance()->iconProvider().type(*this);

	return m_displayType;
}

bool FileSystemInfo::isCaseSensitive() const
{
	return QFSFileEngine(absoluteFilePath()).caseSensitive();
}
