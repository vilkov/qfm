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
	Snapshot createSnapshot(const IFileContainer *container);
	Snapshot createSnapshot(const IFileContainer *container, Snapshot::Container::size_type reserver);

	void scan(Snapshot &snapshot, NodeItem *item, const QString &file, const volatile Flags &aborted) const;
	void scanSoft(Snapshot &snapshot, NodeItem *item, const QString &file, const volatile Flags &aborted) const;

private:
	void scan(InfoListItem *root, const volatile Flags &aborted) const;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESBASETASK_H_ */
