#ifndef IDMNODEQUERYRESULTSREMOVETASK_H_
#define IDMNODEQUERYRESULTSREMOVETASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../../filesystem/tasks/concrete/perform/performremovebasetask.h"


IDM_PLUGIN_NS_BEGIN

class RemoveFilesTask : public PerformRemoveBaseTask
{
public:
	RemoveFilesTask(TasksNode *receiver, const Snapshot &snapshot);

	void run(const volatile Flags &aborted);
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSREMOVETASK_H_ */
