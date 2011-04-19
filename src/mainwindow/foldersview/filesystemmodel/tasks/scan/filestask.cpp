#include "filestask.h"
#include "../../../../../application.h"
#ifndef Q_OS_WIN
#  include <unistd.h>
#  include <sys/types.h>
#endif


FilesTask::FilesTask(Params *params, QObject *controller1) :
	ControlableTask(params, controller1)
#ifndef Q_OS_WIN
    ,m_userId(getuid())
    ,m_groupId(getgid())
#endif
{}

FilesTask::FilesTask(Params *params, QObject *controller1, QObject *controller2) :
	ControlableTask(params, controller1, controller2)
#ifndef Q_OS_WIN
    ,m_userId(getuid())
    ,m_groupId(getgid())
#endif
{}

FilesTask::FilesTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3) :
	ControlableTask(params, controller1, controller2, controller3)
#ifndef Q_OS_WIN
    ,m_userId(getuid())
    ,m_groupId(getgid())
#endif
{}

FileSystemInfo FilesTask::info(const QString &filePath)
{
	FileSystemInfo info(filePath);
	updateInfo(info);
    return info;
}

FileSystemInfo FilesTask::info(const QFileInfo &fileInfo)
{
	FileSystemInfo info(fileInfo);
	updateInfo(info);
    return info;
}

void FilesTask::updateInfo(FileSystemInfo &info)
{
#ifdef Q_OS_WIN
    info.setPermissions(info.permissions());
#else
    info.setPermissions(translatePermissions(info, getuid(), getgid()));
#endif
}

FileSystemInfo FilesTask::getInfo(const QFileInfo &fileInfo) const
{
    FileSystemInfo info(fileInfo);
#ifdef Q_OS_WIN
    info.setPermissions(fileInfo.permissions());
#else
    info.setPermissions(translatePermissions(fileInfo, m_userId, m_groupId));
#endif

    return info;
}

#ifndef Q_OS_WIN
QFile::Permissions FilesTask::translatePermissions(const QFileInfo &fileInfo, uint userId, uint groupId)
{
    QFile::Permissions permissions = fileInfo.permissions();
    QFile::Permissions p = permissions;
    p &= ~(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser);

    if (permissions & QFile::ReadOther || (fileInfo.ownerId() == userId  && permissions & QFile::ReadOwner) ||
										  (fileInfo.groupId() == groupId && permissions & QFile::ReadGroup))
        p |= QFile::ReadUser;

    if (permissions & QFile::WriteOther || (fileInfo.ownerId() == userId  && permissions & QFile::WriteOwner) ||
										   (fileInfo.groupId() == groupId && permissions & QFile::WriteGroup))
        p |= QFile::WriteUser;

    if (permissions & QFile::ExeOther || (fileInfo.ownerId() == userId  && permissions & QFile::ExeOwner) ||
										 (fileInfo.groupId() == groupId && permissions & QFile::ExeGroup))
        p |= QFile::ExeUser;

    return p;
}
#endif
