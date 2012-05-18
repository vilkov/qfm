#include "arcreadarchivetask.h"


ARC_PLUGIN_NS_BEGIN

ReadArchiveTask::ReadArchiveTask(IFileContainer::Holder &container, TasksNode *receiver) :
	FilesBaseTask(receiver),
	m_container(container.take())
{}

void ReadArchiveTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot;
	m_container->scanner()->scan(snapshot, aborted);
	postEvent(new Event(this, Event::ScanComplete, aborted, snapshot));
}

ARC_PLUGIN_NS_END
