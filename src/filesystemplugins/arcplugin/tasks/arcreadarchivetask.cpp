#include "arcreadarchivetask.h"
#include "../archive/arcarchive.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

ReadArchiveTask::ReadArchiveTask(const QString &fileName, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName)
{}

void ReadArchiveTask::run(const volatile bool &aborted)
{
	PScopedPointer<Event> event(new Event(Archive::read(m_fileName), Event::ScanComplete));
	postEvent(event.take());
}

ARC_PLUGIN_NS_END
