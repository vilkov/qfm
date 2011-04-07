#include "filestask.h"
#include "../../../../application.h"
#ifndef Q_OS_WIN
#  include <unistd.h>
#  include <sys/types.h>
#endif


FilesTask::FilesTask(const QString &directory, QObject *receiver) :
	m_directory(directory),
	m_receiver(receiver)
#ifndef Q_OS_WIN
    ,m_userId(getuid())
    ,m_groupId(getgid())
#endif
{}

FileSystemInfo FilesTask::info(const QFileInfo &fileInfo)
{
	IconProvider &iconProvider = Application::instance()->iconProvider();
	iconProvider.lock();

    FileSystemInfo info(fileInfo);
    info.setIcon(iconProvider.iconProvider()->icon(fileInfo));
    info.setPermissions(translatePermissions(fileInfo));
    info.setDisplayType(iconProvider.iconProvider()->type(fileInfo));

    iconProvider.unlock();
    return info;
}

QFile::Permissions FilesTask::translatePermissions(const QFileInfo &fileInfo)
{
    QFile::Permissions permissions = fileInfo.permissions();

#ifdef Q_OS_WIN
    return permissions;
#else
    QFile::Permissions p = permissions;
    p &= ~(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser);

    if (permissions & QFile::ReadOther || (fileInfo.ownerId() == m_userId  && permissions & QFile::ReadOwner) ||
										  (fileInfo.groupId() == m_groupId && permissions & QFile::ReadGroup))
        p |= QFile::ReadUser;

    if (permissions & QFile::WriteOther || (fileInfo.ownerId() == m_userId  && permissions & QFile::WriteOwner) ||
										   (fileInfo.groupId() == m_groupId && permissions & QFile::WriteGroup))
        p |= QFile::WriteUser;

    if (permissions & QFile::ExeOther || (fileInfo.ownerId() == m_userId  && permissions & QFile::ExeOwner) ||
										 (fileInfo.groupId() == m_groupId && permissions & QFile::ExeGroup))
        p |= QFile::ExeUser;

    return p;
#endif
}
