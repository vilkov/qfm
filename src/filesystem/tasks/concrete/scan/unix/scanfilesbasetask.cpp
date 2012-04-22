#include "../scanfilesbasetask.h"

#include <dirent.h>
#include <string.h>


FILE_SYSTEM_NS_BEGIN

ScanFilesImplementation::ScanFilesImplementation()
{}

Snapshot ScanFilesImplementation::createSnapshot(const IFileContainer *container)
{
	return Snapshot(container);
}

Snapshot ScanFilesImplementation::createSnapshot(const IFileContainer *container, Snapshot::Container::size_type reserver)
{
	return Snapshot(container, reserver);
}

void ScanFilesImplementation::scan(Snapshot &snapshot, NodeItem *item, const QString &file, const volatile BaseTask::Flags &aborted) const
{
	Info info(snapshot.container()->location(file), Info::Refresh());

	if (info.isDir())
	{
		PScopedPointer<InfoListItem> subnode(new InfoListItem(snapshot.container(), file));

		scan(subnode.data(), aborted);

		snapshot.push_back(item, subnode.take());
	}
	else
		snapshot.push_back(item, new InfoItem(snapshot.container(), file));
}

void ScanFilesImplementation::scanSoft(Snapshot &snapshot, NodeItem *item, const QString &file, const volatile BaseTask::Flags &aborted) const
{
	Info info(snapshot.container()->location(file), Info::Refresh());

	if (info.isDir())
		snapshot.push_back(item, new InfoListItem(snapshot.container(), file));
	else
		snapshot.push_back(item, new InfoItem(snapshot.container(), file));
}

void ScanFilesImplementation::scan(InfoListItem *root, const volatile BaseTask::Flags &aborted) const
{
	DIR *dir;

	if (dir = opendir(root->container()->location().toUtf8()))
	{
		struct dirent *entry;

		while ((entry = readdir(dir)) != NULL && !aborted)
			if (entry->d_type == DT_DIR)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					PScopedPointer<InfoListItem> subtree(new InfoListItem(root->container(), QString::fromUtf8(entry->d_name)));

					scan(subtree.data(), aborted);
					root->add(subtree.take());
				}
			}
			else
				root->add(new InfoItem(root->container(), QString::fromUtf8(entry->d_name)));

		closedir(dir);
	}
}


ScanFilesBaseTask::ScanFilesBaseTask(TasksNode *receiver) :
	FilesBaseTask(receiver)
{}


ScanFilesExtendedBaseTask::ScanFilesExtendedBaseTask(TasksNode *receiver, ICopyControl::Holder &destination) :
	FilesExtendedBaseTask(receiver, destination)
{}

FILE_SYSTEM_NS_END
