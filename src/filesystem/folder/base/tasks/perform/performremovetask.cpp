#include "performremovetask.h"


FILE_SYSTEM_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, PScopedPointer<InfoListItem> &entries) :
	PerformRemoveBaseTask(receiver),
	m_entries(entries.take())
{}

void PerformRemoveTask::run(const volatile bool &aborted)
{
	remove(m_entries.data(), aborted);

	if (!aborted && !isReceiverDead())
	{
		PScopedPointer<Event> event(new Event(ModelEvent::RemoveFiles, isCanceled(), m_entries));
		QCoreApplication::postEvent(receiver(), event.take());
	}
}

FILE_SYSTEM_NS_END
