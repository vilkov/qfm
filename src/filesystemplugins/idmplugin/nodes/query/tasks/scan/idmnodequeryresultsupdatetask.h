#ifndef IDMNODEQUERYRESULTSUPDATETASK_H_
#define IDMNODEQUERYRESULTSUPDATETASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../../filesystem/tasks/concrete/scan/scanfilesbasetask.h"


IDM_PLUGIN_NS_BEGIN

class UpdateFilesTask : public ScanFilesBaseTask
{
public:
	UpdateFilesTask(TasksNode *receiver, IFileContainer::Holder &container, const TasksNode::TasksItemList &files);

	virtual void run(const volatile Flags &aborted);

protected:
	Snapshot scan(const volatile Flags &aborted);

private:
	Snapshot m_snapshot;
	TasksNode::TasksItemList m_files;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSUPDATETASK_H_ */
