#ifndef IDMNODEQUERYRESULTSREMOVETASK_H_
#define IDMNODEQUERYRESULTSREMOVETASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../../filesystem/tasks/filesystembasetask.h"
#include "../../../../../../filesystem/containers/filesystemsnapshot.h"


IDM_PLUGIN_NS_BEGIN

class RemoveFilesTask : public BaseTask
{
public:
	RemoveFilesTask(TasksNode *receiver, const Snapshot &snapshot);

	void run(const volatile Flags &aborted);

private:
	Snapshot m_snapshot;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSREMOVETASK_H_ */
