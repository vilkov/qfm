#include "scanfilesbasetask.h"
#include "../containers/filesysteminfoentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include <QtCore/QDir>
#include <QtCore/QDirIterator>


FILE_SYSTEM_NS_BEGIN

ScanFilesBaseTask::ScanFilesBaseTask(TasksNode *receiver) :
	BaseTask(receiver)
{}

void ScanFilesBaseTask::scan(InfoListItem *root, const volatile Flags &aborted) const
{
	Info info;
	QDirIterator dirIt(root->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while (!aborted && dirIt.hasNext())
		if ((info = dirIt.next()).isDir())
		{
			PScopedPointer<InfoListItem> subtree(new InfoListItem(info));

			scan(subtree.data(), aborted);
			root->incTotalSize(subtree->totalSize());
			root->add(subtree.take());
		}
		else
		{
			root->add(new InfoEntryItem(info));
			root->incTotalSize(info.fileSize());
		}
}

void ScanFilesBaseTask::scan(InfoListItem *root, const QString &file, const volatile Flags &aborted) const
{
	Info info(root->absoluteFilePath(file));

	if (info.exists())
		if (info.isDir())
		{
			PScopedPointer<InfoListItem> subnode(new InfoListItem(info));

			scan(subnode.data(), aborted);
			root->incTotalSize(subnode->totalSize());
			root->add(subnode.take());
		}
		else
		{
			root->add(new InfoEntryItem(info));
			root->incTotalSize(info.fileSize());
		}
	else
		root->add(new InfoEntryItem(info));
}

InfoItem *ScanFilesBaseTask::scan(const QString &absoluteFilePath, const volatile Flags &aborted) const
{
	Info info(absoluteFilePath);

	if (info.exists() && info.isDir())
	{
		PScopedPointer<InfoListItem> subnode(new InfoListItem(info));

		scan(subnode.data(), aborted);

		return subnode.take();
	}
	else
		return new InfoEntryItem(info);
}

FILE_SYSTEM_NS_END
