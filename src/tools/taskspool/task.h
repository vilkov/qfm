#ifndef TASK_H_
#define TASK_H_

#include "taskspool_ns.h"


TASKSPOOL_NS_BEGIN

class Task
{
public:
	virtual ~Task() {}

protected:
	friend class TaskThread;
	virtual void run(const volatile bool &aborted) = 0;
};

TASKSPOOL_NS_END

#endif /* TASK_H_ */
