#ifndef IDMNODEQUERYRESULTSPERFORMREMOVETASK_H_
#define IDMNODEQUERYRESULTSPERFORMREMOVETASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../default/tasks/perform/default_performremovetask.h"


IDM_PLUGIN_NS_BEGIN

class PerformRemoveTask : public ::VFS::Plugins::Default::PerformRemoveTask
{
public:
	PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot);

protected:
	virtual void run(const volatile Flags &aborted);
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSPERFORMREMOVETASK_H_ */
