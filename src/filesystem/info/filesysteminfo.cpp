#include "filesysteminfo.h"
#include "../../application.h"
#include <QtCore/QFSFileEngine>


FILE_SYSTEM_NS_BEGIN

Info::Info() :
	QFileInfo()
{}

Info::Info(const QString &filePath) :
	QFileInfo(filePath)
{
#ifdef Q_OS_WIN
	m_permissions = info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

Info::Info(const QFileInfo &info) :
	QFileInfo(info)
{
#ifdef Q_OS_WIN
	m_permissions = info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

#ifndef Q_OS_WIN
Info::Info(const QString &filePath, uint userId, uint groupId) :
	QFileInfo(filePath)
{
	translatePermissions(userId, groupId);
}

Info::Info(const QFileInfo &info, uint userId, uint groupId) :
	QFileInfo(info)
{
	translatePermissions(userId, groupId);
}
#endif

bool Info::operator==(const Info &fileInfo) const
{
   return QFileInfo::operator==(fileInfo) &&
   m_displayType == fileInfo.m_displayType &&
   permissions() == fileInfo.permissions();
}

void Info::refresh()
{
	m_displayType = QString();
	m_icon = QIcon();
	QFileInfo::refresh();
}

const QIcon &Info::icon() const
{
	if (m_icon.isNull())
		m_icon = Application::instance()->iconProvider().icon(*this);

	return m_icon;
}

const QString &Info::displayType() const
{
	if (m_displayType.isEmpty())
		m_displayType = Application::instance()->iconProvider().type(*this);

	return m_displayType;
}

bool Info::isCaseSensitive() const
{
	return QFSFileEngine(absoluteFilePath()).caseSensitive();
}

#ifndef Q_OS_WIN
void Info::translatePermissions(uint userId, uint groupId)
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

FILE_SYSTEM_NS_END
