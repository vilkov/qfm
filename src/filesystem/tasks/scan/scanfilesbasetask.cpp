#include "scanfilesbasetask.h"
#include "../containers/filesysteminfoentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include <QtCore/QDir>
#include <QtCore/QDirIterator>


FILE_SYSTEM_NS_BEGIN

ScanFilesBaseTask::ScanFilesBaseTask(TasksNode *receiver) :
	BaseTask(receiver)
{}

void ScanFilesBaseTask::scan(InfoListItem *root, const volatile bool &aborted) const
{
	QFileInfo info;
	QDirIterator dirIt(root->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

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
				root->incTotalSize(subtree->totalSize());
				root->add(subtree.take());
			}
			else
			{
#ifndef Q_OS_WIN
				root->add(new InfoEntryItem(m_permissions.getInfo(info)));
#else
				root->add(new InfoEntryItem(info));
#endif
				root->incTotalSize(info.size());
			}
}

void ScanFilesBaseTask::scan(InfoListItem *root, const QString &file, const volatile bool &aborted) const
{
#ifndef Q_OS_WIN
	Info info(m_permissions.getInfo(root->absoluteFilePath(file)));
#else
	Info info(root->absoluteFilePath(file));
#endif

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
			root->incTotalSize(info.size());
		}
	else
		root->add(new InfoEntryItem(info));
}

InfoItem *ScanFilesBaseTask::scan(const QString &absoluteFilePath, const volatile bool &aborted) const
{
#ifndef Q_OS_WIN
	Info info(m_permissions.getInfo(absoluteFilePath));
#else
	Info info(absoluteFilePath);
#endif

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
