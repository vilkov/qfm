#ifndef TASKPOOL_H_
#define TASKPOOL_H_

#include <list>
#include <stdint.h>
#include "taskthread.h"
#include "../threads/pmutex.h"


TASKSPOOL_NS_BEGIN

class TaskPool
{
public:
	TaskPool(int32_t maxThreads);
	~TaskPool();

	bool handleImmediately(Task *task);
	void handle(Task *task);
	bool remove(Task *task);
	void clear();

protected:
	friend class TaskThread;
	Task *nextTask(TaskThread *thread);

private:
	PMutex m_mutex;
    uint32_t m_maxThreads;
    std::list<Task*> m_tasks;
	std::list<TaskThread*> m_threads;
	std::list<TaskThread*> m_freeThreads;
};

TASKSPOOL_NS_END

#endif /* TASKPOOL_H_ */
