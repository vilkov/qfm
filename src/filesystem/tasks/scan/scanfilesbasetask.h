#ifndef SCANFILESBASETASK_H_
#define SCANFILESBASETASK_H_

#include "../filesystembasetask.h"
#include "../containers/filesysteminfolistitem.h"
#ifndef Q_OS_WIN
#	include "../tools/taskpermissionscache.h"
#endif


FILE_SYSTEM_NS_BEGIN

class ScanFilesBaseTask : public BaseTask
{
public:
	ScanFilesBaseTask(TasksNode *receiver);

protected:
	void scan(InfoListItem *root, const volatile bool &aborted) const;
	void scan(InfoListItem *root, const QString &file, const volatile bool &aborted) const;

private:
#ifndef Q_OS_WIN
	TaskPermissionsCache m_permissions;
#endif
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESBASETASK_H_ */
