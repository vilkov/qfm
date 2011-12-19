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
	typedef std::list<TaskThread*> ThreadList;

public:
	TaskPool(ThreadList::size_type maxThreads);
	~TaskPool();

	void handle(Task *task);
	void clear();

protected:
	friend class TaskThread;
	Task *nextTask(TaskThread *thread);

private:
	PMutex m_mutex;
    bool m_clearing;
    ThreadList m_threads;
    ThreadList m_freeThreads;
    std::list<Task*> m_tasks;
    ThreadList::size_type m_maxThreads;
};

TASKSPOOL_NS_END

#endif /* TASKPOOL_H_ */
