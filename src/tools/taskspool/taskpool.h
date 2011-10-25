#ifndef TASKPOOL_H_
#define TASKPOOL_H_

#include <list>
#include "taskspool_ns.h"
#include "taskthread.h"
#include "task.h"
#include "../types/ptypes.h"
#include "../threads/pmutex.h"


TASKSPOOL_NS_BEGIN

class TaskPool
{
public:
	TaskPool(types::int32_t maxThreads);
	~TaskPool();

	void handle(Task *task);
	void clear();

protected:
	friend class TaskThread;
	Task *nextTask(TaskThread *thread);

private:
	PMutex m_mutex;
    bool m_clearing;
    types::int32_t m_maxThreads;
    std::list<Task*> m_tasks;
	std::list<TaskThread*> m_threads;
	std::list<TaskThread*> m_freeThreads;
};

TASKSPOOL_NS_END

#endif /* TASKPOOL_H_ */
