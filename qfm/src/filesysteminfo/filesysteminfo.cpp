#include "filesysteminfo.h"
#include "../application.h"
#include <QtCore/QFSFileEngine>


FileSystemInfo::FileSystemInfo() :
	QFileInfo()
{}

FileSystemInfo::FileSystemInfo(const QString &filePath) :
	QFileInfo(filePath)
{
#ifdef Q_OS_WIN
	m_permissions = info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

FileSystemInfo::FileSystemInfo(const QFileInfo &info) :
	QFileInfo(info)
{
#ifdef Q_OS_WIN
	m_permissions = info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

#ifndef Q_OS_WIN
FileSystemInfo::FileSystemInfo(const QString &filePath, uint userId, uint groupId) :
	QFileInfo(filePath)
{
	translatePermissions(userId, groupId);
}

FileSystemInfo::FileSystemInfo(const QFileInfo &info, uint userId, uint groupId) :
	QFileInfo(info)
{
	translatePermissions(userId, groupId);
}
#endif

bool FileSystemInfo::operator==(const FileSystemInfo &fileInfo) const
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

#ifndef Q_OS_WIN
void FileSystemInfo::translatePermissions(uint userId, uint groupId)
{
	QFile::Permissions perm = QFileInfo::permissions();
	QFile::Permissions p = perm;
	p &= ~(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser);

	if (perm & QFile::ReadOther || (ownerId() == userId  && perm & QFile::ReadOwner) ||
                                   (QFileInfo::groupId() == groupId && perm & QFile::ReadGroup))
		p |= QFile::ReadUser;

	if (perm & QFile::WriteOther || (ownerId() == userId  && perm & QFile::WriteOwner) ||
                                    (QFileInfo::groupId() == groupId && perm & QFile::WriteGroup))
		p |= QFile::WriteUser;

	if (perm & QFile::ExeOther || (ownerId() == userId  && perm & QFile::ExeOwner) ||
                                  (QFileInfo::groupId() == groupId && perm & QFile::ExeGroup))
		p |= QFile::ExeUser;

	m_permissions = p;
}
#endif
