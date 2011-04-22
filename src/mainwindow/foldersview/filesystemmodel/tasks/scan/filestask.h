#ifndef FILESTASK_H_
#define FILESTASK_H_

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include "../../filesysteminfo.h"
#ifndef Q_OS_WIN
#  include <unistd.h>
#  include <sys/types.h>
#endif


class FilesTask
{
public:
	static FileSystemInfo info(const QString &filePath)
	{
		FileSystemInfo info(filePath);
		updateInfo(info);
	    return info;
	}
	static FileSystemInfo info(const QFileInfo &fileInfo)
	{
		FileSystemInfo info(fileInfo);
		updateInfo(info);
	    return info;
	}

protected:
	static void updateInfo(FileSystemInfo &info)
	{
	#ifdef Q_OS_WIN
	   	info.setPermissions(info.permissions());
	#else
	   	info.setPermissions(translatePermissions(info, getuid(), getgid()));
	#endif
	}
#ifndef Q_OS_WIN
    static QFile::Permissions translatePermissions(const QFileInfo &fileInfo, uint userId, uint groupId)
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
};


template <typename BaseClass>
class TemplateFilesTask : public BaseClass, public FilesTask
{
public:
	typedef BaseClass parent_class;
	typedef typename parent_class::Params Params;

public:
	TemplateFilesTask(Params *params) :
		parent_class(params)
	#ifndef Q_OS_WIN
		,m_userId(getuid())
		,m_groupId(getgid())
	#endif
	{}


protected:
	FileSystemInfo getInfo(const QFileInfo &fileInfo) const
	{
	    FileSystemInfo info(fileInfo);
	#ifdef Q_OS_WIN
	    info.setPermissions(fileInfo.permissions());
	#else
	    info.setPermissions(translatePermissions(fileInfo, m_userId, m_groupId));
	#endif

	    return info;
	}

private:
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

#endif /* FILESTASK_H_ */
