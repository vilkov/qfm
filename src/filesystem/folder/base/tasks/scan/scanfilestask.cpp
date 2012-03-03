#include "scanfilestask.h"
#include "../../items/filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesTask::ScanFilesTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &files) :
	ScanFilesBaseTask(receiver),
	m_snapshot(createSnapshot(container, files.size())),
	m_files(files)
{}

Snapshot ScanFilesTask::scan(const volatile Flags &aborted)
{
	for (TasksNode::TasksItemList::size_type i = 0, size = m_files.size(); i < size && !aborted; ++i)
		ScanFilesBaseTask::scan(m_snapshot, m_files.at(i), &static_cast<FileSystemBaseItem*>(m_files.at(i))->info(), aborted);

	return m_snapshot;
}

FILE_SYSTEM_NS_END
