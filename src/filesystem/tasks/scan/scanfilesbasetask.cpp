#include "scanfilesbasetask.h"
#include "../containers/filesysteminfoentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include <QtCore/QDir>
#include <QtCore/QDirIterator>


FILE_SYSTEM_NS_BEGIN

ScanFilesBaseTask::ScanFilesBaseTask(TasksNode *receiver) :
	BaseTask(receiver)
{}

InfoListItem *ScanFilesBaseTask::scan(const QString &root, const volatile bool &aborted) const
{
	PScopedPointer<InfoListItem> rootItem(new InfoListItem(root));
	scan(rootItem.data(), aborted);

	return rootItem.take();
}

InfoListItem *ScanFilesBaseTask::scan(const QString &root, const QStringList &files, const volatile bool &aborted) const
{
	PScopedPointer<InfoListItem> rootItem(new InfoListItem(root));

	for (QStringList::size_type i = 0, size = files.size(); i < size && !aborted; ++i)
	{
#ifndef Q_OS_WIN
		Info info(m_permissions.getInfo(rootItem->absoluteFilePath(files.at(i))));
#else
		Info info(rootItem->absoluteFilePath(files.at(i)));
#endif

		if (info.exists())
			if (info.isDir())
			{
				PScopedPointer<InfoListItem> subnode(new InfoListItem(info));

				scan(subnode.data(), aborted);
				rootItem->incTotalSize(subnode->totalSize());
				rootItem->add(subnode.take());
			}
			else
			{
				rootItem->add(new InfoEntryItem(info));
				rootItem->incTotalSize(info.size());
			}
		else
			rootItem->add(new InfoEntryItem(info));
	}

	return rootItem.take();
}

void ScanFilesBaseTask::scan(InfoListItem *node, const volatile bool &aborted) const
{
	QFileInfo info;
	QDirIterator dirIt(node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while (!isCanceled() && !aborted && !isReceiverDead() && dirIt.hasNext())
		if (!(info = dirIt.next()).isSymLink())
			if (info.isDir())
			{
#ifndef Q_OS_WIN
				PScopedPointer<InfoListItem> subtree(new InfoListItem(m_permissions.getInfo(info)));
#else
				PScopedPointer<InfoListItem> subtree(new InfoListItem(info));
#endif
				scan(subtree.data(), aborted);
				node->incTotalSize(subtree->totalSize());
				node->add(subtree.take());
			}
			else
			{
#ifndef Q_OS_WIN
				node->add(new InfoEntryItem(m_permissions.getInfo(info)));
#else
				node->add(new InfoEntryItem(info));
#endif
				node->incTotalSize(info.size());
			}
}

FILE_SYSTEM_NS_END
