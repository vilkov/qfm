#include "idmnodequeryresultsremovefilestask.h"


IDM_PLUGIN_NS_BEGIN

RemoveFilesTask::RemoveFilesTask(TasksNode *receiver, const Snapshot &snapshot) :
	BaseTask(receiver),
	m_snapshot(snapshot)
{}

void RemoveFilesTask::run(const volatile Flags &aborted)
{
//	Snapshot snapshot = remove(aborted);
//	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::RemoveFiles), aborted, snapshot));
}

IDM_PLUGIN_NS_END
