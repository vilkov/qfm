#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "../../containers/filesystemupdateslist.h"
#include "../../../tasks/filesystemfolderbasetask.h"
#ifndef Q_OS_WIN
#	include "../../../tasks/taskpermissionscache.h"
#endif


FILE_SYSTEM_NS_BEGIN

class UpdateFilesTask : public FolderBaseTask
{
public:
	class Event : public ModelEvent
	{
	public:
		Event(bool isLastEvent, const UpdatesList &updates) :
			ModelEvent(UpdateFiles),
			isLastEvent(isLastEvent),
			updates(updates)
		{}

		bool isLastEvent;
		UpdatesList updates;
	};

public:
	UpdateFilesTask(QObject *receiver, const Info &info, const UpdatesList &updates);

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
