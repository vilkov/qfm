#ifndef IDMNODEQUERYRESULTSSCANTASK_H_
#define IDMNODEQUERYRESULTSSCANTASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../../filesystem/tasks/concrete/scan/scanfilesbasetask.h"


IDM_PLUGIN_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	ScanFilesTask(TasksNode *receiver, const IFileContainer *container, const TasksNode::TasksItemList &files);

	virtual void run(const volatile Flags &aborted);

protected:
	Snapshot scan(const volatile Flags &aborted);

private:
	Snapshot m_snapshot;
	TasksNode::TasksItemList m_files;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSSCANTASK_H_ */
