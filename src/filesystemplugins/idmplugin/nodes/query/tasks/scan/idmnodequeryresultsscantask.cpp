#include "idmnodequeryresultsscantask.h"
#include "../../items/idmqueryresultpathitem.h"


IDM_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &files) :
	ScanFilesBaseTask(receiver),
	m_snapshot(createSnapshot(container, files.size())),
	m_files(files)
{}

void ScanFilesTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot = scan(aborted);
	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::ScanFilesForRemove), aborted, snapshot));
}

Snapshot ScanFilesTask::scan(const volatile Flags &aborted)
{
	for (TasksNode::TasksItemList::size_type i = 0, size = m_files.size(); i < size && !aborted; ++i)
		ScanFilesBaseTask::scan(m_snapshot, m_files.at(i), static_cast<QueryResultPathItem *>(m_files.at(i))->fileName(), aborted);

	return m_snapshot;
}

IDM_PLUGIN_NS_END
