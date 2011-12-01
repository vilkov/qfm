#ifndef PERFORMREMOVETASK_H_
#define PERFORMREMOVETASK_H_

#include "performremovebasetask.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveTask : public PerformRemoveBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTask)

public:
	PerformRemoveTask(TasksNode *receiver, PScopedPointer<InfoListItem> &entries);

	virtual void run(const volatile bool &aborted);

private:
	PScopedPointer<InfoListItem> m_entries;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVETASK_H_ */
