#include "performremovetask.h"


FILE_SYSTEM_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const ScanedFiles &entries) :
	PerformRemoveBaseTask(receiver),
	m_entries(entries)
{}

void PerformRemoveTask::run(const volatile bool &aborted)
{
	remove(m_entries, aborted);

	if (!aborted && !isReceiverDead())
	{
		PScopedPointer<Event> event(new Event(ModelEvent::RemoveFiles, this, isCanceled(), m_entries));
		QCoreApplication::postEvent(receiver(), event.take());
	}
}

FILE_SYSTEM_NS_END
