#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "../../events/filesystemmodelevent.h"
#include "../../containers/filesystemupdateslist.h"
#include "../../../../tasks/concrete/scan/scanfilesbasetask.h"


FILE_SYSTEM_NS_BEGIN

class UpdateFilesTask : public ScanFilesBaseTask
{
public:
	class Event : public BaseTask::Event
	{
	public:
		Event(BaseTask *task, bool isLastEvent, const UpdatesList &updates, bool canceled = false) :
			BaseTask::Event(task, static_cast<Type>(ModelEvent::UpdateFiles), canceled),
			isLastEvent(isLastEvent),
			updates(updates)
		{}

		bool isLastEvent;
		UpdatesList updates;
	};

public:
	UpdateFilesTask(TasksNode *receiver, IFileContainer::Holder &container, const UpdatesList &updates);

	virtual void run(const volatile Flags &aborted);

private:
	UpdatesList m_updates;
	IFileContainer::Holder m_container;
};

FILE_SYSTEM_NS_END

#endif /* UPDATEFILESTASK_H_ */
