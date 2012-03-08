#include "idmnodequeryresultsupdatetask.h"
#include "../../items/idmqueryresultpathvalueitem.h"


IDM_PLUGIN_NS_BEGIN

UpdateFilesTask::UpdateFilesTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &files) :
	ScanFilesBaseTask(receiver),
	m_snapshot(createSnapshot(container, files.size())),
	m_files(files)
{}

void UpdateFilesTask::run(const volatile Flags &aborted)
{
	Snapshot snapshot = scan(aborted);
	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::ScanFilesForRemove), aborted, snapshot));
}

Snapshot UpdateFilesTask::scan(const volatile Flags &aborted)
{
	for (TasksNode::TasksItemList::size_type i = 0, size = m_files.size(); i < size && !aborted; ++i)
		ScanFilesBaseTask::scan(m_snapshot, m_files.at(i), &static_cast<QueryResultPathValueItem*>(m_files.at(i))->info(), aborted);

	return m_snapshot;
}

IDM_PLUGIN_NS_END
