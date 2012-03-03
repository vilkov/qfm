#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QStringList>
#include "../../events/filesystemmodelevent.h"
#include "../../../../tasks/concrete/scan/scanfilesbasetask.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	ScanFilesTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &files);

protected:
	Snapshot scan(const volatile Flags &aborted);

private:
	Snapshot m_snapshot;
	TasksNode::TasksItemList m_files;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
