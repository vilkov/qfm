#include "scanfilestask.h"
#include "../../items/filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesTask::ScanFilesTask(TasksNode *receiver, const TasksNode::TasksItemList &files) :
	ScanFilesBaseTask(receiver),
	m_files(files)
{}

ScanedFiles ScanFilesTask::scan(const volatile Flags &aborted) const
{
	InfoItem *item;
	ScanedFiles res(m_files.size());

	for (TasksNode::TasksItemList::size_type i = 0, size = m_files.size(); i < size && !aborted; ++i)
	{
		item = ScanFilesBaseTask::scan(static_cast<FileSystemBaseItem*>(m_files.at(i))->info().absoluteFilePath(), aborted);
		res.push_back(m_files.at(i), item);
	}

	return res;
}

FILE_SYSTEM_NS_END
