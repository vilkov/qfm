#ifndef SCANFILESBASETASK_H_
#define SCANFILESBASETASK_H_

#include "../filesystemfilesbasetask.h"
#include "../filesystemfilesextendedbasetask.h"
#include "../containers/filesysteminfolistitem.h"
#include "../../filesystemtasksnode.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesImplementation
{
public:
	ScanFilesImplementation();

public:
	Snapshot createSnapshot(const IFileContainer *container);
	Snapshot createSnapshot(const IFileContainer *container, Snapshot::Container::size_type reserver);

	void scan(Snapshot &snapshot, NodeItem *item, const QString &file, const volatile BaseTask::Flags &aborted) const;
	void scanSoft(Snapshot &snapshot, NodeItem *item, const QString &file, const volatile BaseTask::Flags &aborted) const;

private:
	void scan(InfoListItem *root, const volatile BaseTask::Flags &aborted) const;
};


class ScanFilesBaseTask : public FilesBaseTask, protected ScanFilesImplementation
{
public:
	ScanFilesBaseTask(TasksNode *receiver);
};


class ScanFilesExtendedBaseTask : public FilesExtendedBaseTask, protected ScanFilesImplementation
{
public:
	ScanFilesExtendedBaseTask(TasksNode *receiver, ICopyControl::Holder &destination);
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESBASETASK_H_ */
