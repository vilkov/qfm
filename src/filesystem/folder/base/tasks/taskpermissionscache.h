#ifndef TASKPERMISSIONSCACHE_H_
#define TASKPERMISSIONSCACHE_H_

#include <unistd.h>
#include <sys/types.h>
#include "../../../info/filesystemfoldernodeinfo.h"


FILE_SYSTEM_NS_BEGIN

class TaskPermissionsCache
{
public:
	TaskPermissionsCache() :
		m_userId(getuid()),
		m_groupId(getgid())
	{}

	Info getInfo(const QFileInfo &fileInfo) const
	{
	#ifdef Q_OS_WIN
	    return Info(fileInfo);
	#else
	    return Info(fileInfo, m_userId, m_groupId);
	#endif
	}

private:
    uint m_userId;
    uint m_groupId;
};

FILE_SYSTEM_NS_END

#endif /* TASKPERMISSIONSCACHE_H_ */
