#ifndef TASK_H_
#define TASK_H_

#include "ns_taskspool.h"
#include "../memory/memory_manager.h"


TASKSPOOL_NS_BEGIN

class Task : public MemoryManagerTag
{
public:
	virtual ~Task() {}

	virtual void run(const volatile bool &stopedFlag) = 0;
};

TASKSPOOL_NS_END

#endif /* TASK_H_ */
