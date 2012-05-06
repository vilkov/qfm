#ifndef IDMNODEQUERYRESULTSPERFORMREMOVETASK_H_
#define IDMNODEQUERYRESULTSPERFORMREMOVETASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../default/tasks/perform/defaultperformremovetask.h"


IDM_PLUGIN_NS_BEGIN

class PerformRemoveTask : public ::FileSystem::Plugins::Default::PerformRemoveTask
{
public:
	PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot);

protected:
	virtual void run(const volatile Flags &aborted);
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSPERFORMREMOVETASK_H_ */
