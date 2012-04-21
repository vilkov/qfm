#include "performremovetask.h"


FILE_SYSTEM_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot) :
	PerformRemoveBaseTask(receiver, snapshot)
{}

void PerformRemoveTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot = remove(aborted);
	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::RemoveFiles), aborted, snapshot));
}

FILE_SYSTEM_NS_END
