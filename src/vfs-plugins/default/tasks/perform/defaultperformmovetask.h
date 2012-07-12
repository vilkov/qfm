#ifndef PERFORMMOVETASK_H_
#define PERFORMMOVETASK_H_

#include "defaultperformcopytask.h"


DEFAULT_PLUGIN_NS_BEGIN

class PerformMoveTask : public PerformCopyTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformMoveTask)

public:
	PerformMoveTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot);

protected:
	virtual void copyFile(const IFileContainer *destination, WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
};

DEFAULT_PLUGIN_NS_END

#endif /* PERFORMMOVETASK_H_ */
