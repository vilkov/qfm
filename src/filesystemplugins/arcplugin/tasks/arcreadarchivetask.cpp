#include "arcreadarchivetask.h"
#include "../archive/arcarchive.h"
#include "../../../tools/pointers/pscopedpointer.h"


ARC_PLUGIN_NS_BEGIN

ReadArchiveTask::ReadArchiveTask(const QString &fileName, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName)
{}

void ReadArchiveTask::run(const volatile Flags &aborted)
{
	Archive::State *state;
	PScopedPointer<Event> event(new Event(this, false));

	if (const Archive *archive = Archive::archive(m_fileName, &state))
	{
		event->contents = archive->readAll(state, aborted);
		archive->endRead(state);
	}

	event->canceled = aborted;
	postEvent(event.take());
}

ARC_PLUGIN_NS_END
