#include "arc_readarchivetask.h"


ARC_PLUGIN_NS_BEGIN

ReadArchiveTask::ReadArchiveTask(const IFileContainer *container, TasksNode *receiver) :
	FilesBaseTask(receiver),
	m_container(container)
{}

void ReadArchiveTask::run(const volatile Flags &aborted)
{
	QString error;
	Snapshot snapshot(m_container);
	IFileContainerScanner::ScanArguments args = {snapshot, aborted};
	m_container->scanner()->scan(args, error);
	postEvent(new Event(this, Event::ScanComplete, error, aborted, snapshot));
}

ARC_PLUGIN_NS_END
