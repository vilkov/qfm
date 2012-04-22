#ifndef PERFORMREMOVETASK_H_
#define PERFORMREMOVETASK_H_

#include "../../events/filesystemmodelevent.h"
#include "../../../../tasks/concrete/perform/performremovebasetask.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveTask : public PerformRemoveBaseTask
{
public:
	PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot);

	virtual void run(const volatile Flags &aborted);
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETASK_H_ */
