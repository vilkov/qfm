#include "arcreadarchivetask.h"


ARC_PLUGIN_NS_BEGIN

ReadArchiveTask::ReadArchiveTask(const IFileContainer *container, TasksNode *receiver) :
	FilesBaseTask(receiver),
	m_container(container)
{}

void ReadArchiveTask::run(const volatile Flags &aborted)
{
	QString error;
	Snapshot snapshot(m_container);
	m_container->scanner()->scan(snapshot, aborted, error);
	postEvent(new Event(this, Event::ScanComplete, error, aborted, snapshot));
}

ARC_PLUGIN_NS_END
