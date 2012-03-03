#ifndef SCANFILESBASETASK_H_
#define SCANFILESBASETASK_H_

#include "../filesystemfilesbasetask.h"
#include "../containers/filesysteminfolistitem.h"
#include "../../filesystemtasksnode.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesBaseTask : public FilesBaseTask
{
public:
	ScanFilesBaseTask(TasksNode *receiver);

protected:
	Snapshot createSnapshot(IFileContainer::Holder &container);
	Snapshot createSnapshot(IFileContainer::Holder &container, Snapshot::Container::size_type reserver);

	void scan(InfoListItem *root, const volatile Flags &aborted) const;
	void scan(InfoListItem *root, const QString &fileName, const volatile Flags &aborted) const;
	void scan(Snapshot &snapshot, FileSystemItem *item, const IFileInfo *file, const volatile Flags &aborted) const;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESBASETASK_H_ */
