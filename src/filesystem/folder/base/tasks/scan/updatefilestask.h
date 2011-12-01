#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "../../containers/filesystemupdateslist.h"
#include "../filesystemfolderbasetask.h"
#ifndef Q_OS_WIN
#	include "../taskpermissionscache.h"
#endif


FILE_SYSTEM_NS_BEGIN

class UpdateFilesTask : public FolderBaseTask
{
public:
	class Event : public FolderBaseTask::Event
	{
	public:
		Event(bool isLastEvent, const UpdatesList &updates) :
			FolderBaseTask::Event(UpdateFiles),
			isLastEvent(isLastEvent),
			updates(updates)
		{}

		bool isLastEvent;
		UpdatesList updates;
	};

public:
	UpdateFilesTask(TaskNode *receiver, const Info &info, const UpdatesList &updates);

	virtual void run(const volatile bool &aborted);

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
