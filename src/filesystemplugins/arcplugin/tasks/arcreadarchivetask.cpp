#include "arcreadarchivetask.h"
#include "../archive/arcarchive.h"
#include "../../../tools/pointers/pscopedpointer.h"


ARC_PLUGIN_NS_BEGIN

ReadArchiveTask::ReadArchiveTask(const QString &fileName, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName)
{}

void ReadArchiveTask::run(const volatile bool &aborted)
{
	PScopedPointer<Event> event(new Event(Event::ScanComplete));

	event->contents = Archive::read(m_fileName, aborted, isCanceled());
	event->canceled = isCanceled();

	if (!aborted && !isReceiverDead())
		postEvent(event.take());
	else
		qDeleteAll(event->contents.items);
}

ARC_PLUGIN_NS_END
