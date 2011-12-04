#ifndef PERFORMREMOVETASK_H_
#define PERFORMREMOVETASK_H_

#include "../../events/filesystemmodelevent.h"
#include "../../../../tasks/perform/performremovebasetask.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveTask : public PerformRemoveBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTask)

public:
	class Event : public PerformRemoveBaseTask::Event
	{
	public:
		Event(ModelEvent::Type type, BaseTask *task, bool canceled, PScopedPointer<InfoListItem> &entries) :
			PerformRemoveBaseTask::Event(static_cast<Type>(type)),
			task(task),
			entries(entries.take()),
			canceled(canceled)
		{}

		BaseTask *task;
		PScopedPointer<InfoListItem> entries;
		bool canceled;
	};

public:
	PerformRemoveTask(TasksNode *receiver, PScopedPointer<InfoListItem> &entries);

	virtual void run(const volatile bool &aborted);

private:
	PScopedPointer<InfoListItem> m_entries;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETASK_H_ */
