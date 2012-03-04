#include "../scanfilesbasetask.h"

#include <dirent.h>
#include <string.h>


FILE_SYSTEM_NS_BEGIN

ScanFilesBaseTask::ScanFilesBaseTask(TasksNode *receiver) :
	FilesBaseTask(receiver)
{}

Snapshot ScanFilesBaseTask::createSnapshot(IFileContainer::Holder &container)
{
	return Snapshot(container);
}

Snapshot ScanFilesBaseTask::createSnapshot(IFileContainer::Holder &container, Snapshot::Container::size_type reserver)
{
	return Snapshot(container, reserver);
}

void ScanFilesBaseTask::scan(InfoListItem *root, const volatile Flags &aborted) const
{
	DIR *dir;

	if (dir = opendir(root->container().location().toUtf8()))
	{
		struct dirent *entry;

		while ((entry = readdir(dir)) != NULL && !aborted)
			if (entry->d_type == DT_DIR)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					PScopedPointer<InfoListItem> subtree(new InfoListItem(root->container().location(QString::fromUtf8(entry->d_name))));

					scan(subtree.data(), aborted);
					root->add(subtree.take());
				}
			}
			else
				root->add(new InfoItem(root->container().location(QString::fromUtf8(entry->d_name))));

		closedir(dir);
	}
}

void ScanFilesBaseTask::scan(InfoListItem *root, const QString &fileName, const volatile Flags &aborted) const
{
	if (root->container().contains(fileName))
	{
		Info info(root->container().location(fileName));

		if (info.isDir())
		{
			PScopedPointer<InfoListItem> subnode(new InfoListItem(info));

			scan(subnode.data(), aborted);
			root->add(subnode.take());
		}
		else
			root->add(new InfoItem(info));
	}
	else
		root->add(new InfoItem(root->container().location(fileName)));
}

void ScanFilesBaseTask::scan(Snapshot &snapshot, FileSystemItem *item, const IFileInfo *file, const volatile Flags &aborted) const
{
	if (file->isDir())
	{
		PScopedPointer<InfoListItem> subnode(new InfoListItem(snapshot.container()->location(file->fileName())));

		scan(subnode.data(), aborted);

		snapshot.push_back(item, subnode.take());
	}
	else
		snapshot.push_back(item, new InfoItem(snapshot.container()->location(file->fileName())));
}

FILE_SYSTEM_NS_END
