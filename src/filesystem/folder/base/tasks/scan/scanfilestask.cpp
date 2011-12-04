#include "scanfilestask.h"
#include "../../items/filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesTask::ScanFilesTask(TasksNode *receiver, const Info &root, const TasksNode::TasksItemList &files) :
	ScanFilesBaseTask(receiver),
	m_root(root),
	m_files(files)
{}

void ScanFilesTask::run(const volatile bool &aborted)
{
	PScopedPointer<InfoListItem> root(new InfoListItem(m_root));

	for (TasksNode::TasksItemList::size_type i = 0, size = m_files.size(); i < size; ++i)
		scan(root.data(), static_cast<FileSystemBaseItem*>(m_files.at(i))->info().fileName(), aborted);

	m_subnode.swap(root);
}

FILE_SYSTEM_NS_END
