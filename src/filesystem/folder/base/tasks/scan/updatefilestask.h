#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "../../events/filesystemmodelevent.h"
#include "../../containers/filesystemupdateslist.h"
#include "../../../../tasks/scan/scanfilesbasetask.h"


FILE_SYSTEM_NS_BEGIN

class UpdateFilesTask : public ScanFilesBaseTask
{
public:
	class Event : public ScanFilesBaseTask::Event
	{
	public:
		Event(bool isLastEvent, const UpdatesList &updates) :
			ScanFilesBaseTask::Event(static_cast<Type>(ModelEvent::UpdateFiles)),
			isLastEvent(isLastEvent),
			updates(updates)
		{}

		bool isLastEvent;
		UpdatesList updates;
	};

public:
	UpdateFilesTask(TasksNode *receiver, const Info &info, const UpdatesList &updates);

	virtual void run(const volatile Flags &aborted);

private:
	Info m_info;
	UpdatesList m_updates;
};

FILE_SYSTEM_NS_END

#endif /* UPDATEFILESTASK_H_ */
