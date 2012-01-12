#include "idmnodequeryresultsscantask.h"
#include "../../items/idmqueryresultpathvalueitem.h"
#include "../../../../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(TasksNode *receiver, const TasksNode::TasksItemList &files) :
	ScanFilesBaseTask(receiver),
	m_files(files)
{}

void ScanFilesTask::run(const volatile Flags &aborted)
{
	PScopedPointer<Event> event(new Event(this, ModelEvent::ScanFilesForRemove));
	event->files = scan(aborted);
	event->canceled = aborted;
	postEvent(event.take());
}

ScanedFiles ScanFilesTask::scan(const volatile Flags &aborted) const
{
	InfoItem *item;
	ScanedFiles res(m_files.size());

	for (TasksNode::TasksItemList::size_type i = 0, size = m_files.size(); i < size; ++i)
	{
		item = ScanFilesBaseTask::scan(static_cast<QueryResultPathValueItem*>(m_files.at(i))->info().absoluteFilePath(), aborted);
		res.push_back(m_files.at(i), item);
	}

	return res;
}

IDM_PLUGIN_NS_END
