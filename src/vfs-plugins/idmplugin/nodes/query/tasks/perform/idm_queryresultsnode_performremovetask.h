#ifndef IDM_QUERYRESULTSNODE_PERFORMREMOVETASK_H_
#define IDM_QUERYRESULTSNODE_PERFORMREMOVETASK_H_

#include "../../events/idm_queryresultsmodelevents.h"
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

#endif /* IDM_QUERYRESULTSNODE_PERFORMREMOVETASK_H_ */
