#include "filesystemfoldernodeinfo.h"
#include "../../../application.h"
#include <QtCore/QFSFileEngine>


FILE_SYSTEM_NS_BEGIN

Info::Info() :
	m_info()
{}

Info::Info(const QString &filePath) :
	m_info(filePath)
{
#ifdef Q_OS_WIN
	m_permissions = info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

Info::Info(const QFileInfo &info) :
	m_info(info)
{
#ifdef Q_OS_WIN
	m_permissions = info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

#ifndef Q_OS_WIN
Info::Info(const QString &filePath, uint userId, uint groupId) :
	m_info(filePath)
{
	translatePermissions(userId, groupId);
}

Info::Info(const QFileInfo &info, uint userId, uint groupId) :
	m_info(info)
{
	translatePermissions(userId, groupId);
}
#endif

void Info::refresh()
{
	m_displayType = QString();
	m_icon = QIcon();
	m_info.refresh();
}

const QIcon &Info::icon() const
{
	if (m_icon.isNull())
		m_icon = Application::instance()->iconProvider().icon(m_info);

	return m_icon;
}

const QString &Info::displayType() const
{
	if (m_displayType.isEmpty())
		m_displayType = Application::instance()->iconProvider().type(m_info);

	return m_displayType;
}

bool Info::isCaseSensitive() const
{
	return QFSFileEngine(absoluteFilePath()).caseSensitive();
}

#ifndef Q_OS_WIN
void Info::translatePermissions(uint userId, uint groupId)
{
	QFile::Permissions perm = m_info.permissions();
	QFile::Permissions p = perm;
	p &= ~(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser);

	if (perm & QFile::ReadOther || (m_info.ownerId() == userId  && perm & QFile::ReadOwner) ||
                                   (m_info.groupId() == groupId && perm & QFile::ReadGroup))
		p |= QFile::ReadUser;

	if (perm & QFile::WriteOther || (m_info.ownerId() == userId  && perm & QFile::WriteOwner) ||
                                    (m_info.groupId() == groupId && perm & QFile::WriteGroup))
		p |= QFile::WriteUser;

	if (perm & QFile::ExeOther || (m_info.ownerId() == userId  && perm & QFile::ExeOwner) ||
                                  (m_info.groupId() == groupId && perm & QFile::ExeGroup))
		p |= QFile::ExeUser;

	m_permissions = p;
}
#endif

FILE_SYSTEM_NS_END
