#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "../destcontrolabletask.h"
#ifndef Q_OS_WIN
#	include "../taskpermissionscache.h"
#endif
#include "../../containers/filesystemupdateslist.h"


FILE_SYSTEM_NS_BEGIN

class UpdateFilesTask : public DestControlableTask
{
public:
	typedef ModelEvents::UpdateFilesEvent Event;

public:
	UpdateFilesTask(QObject *receiver, const Info &info, const UpdatesList &updates);

	virtual void run(const volatile bool &stopedFlag);

private:
	Info m_info;
	UpdatesList m_updates;

#ifndef Q_OS_WIN
private:
	TaskPermissionsCache m_permissions;
#endif
};

FILE_SYSTEM_NS_END

#endif /* UPDATEFILESTASK_H_ */
