#ifndef TASKTHREAD_H_
#define TASKTHREAD_H_

#include "ns_taskspool.h"
#include "task.h"
#include "../threads/pmutex.h"
#include "../threads/pthread.h"
#include "../threads/pcondition.h"


TASKSPOOL_NS_BEGIN

class TaskPool;

class TaskThread : public PThread
{
public:
    TaskThread(TaskPool *pool, Task *task);
    ~TaskThread();

    void handle(Task *task);
    void terminate();

protected:
    virtual void run();

private:
    Task *m_task;
    TaskPool *m_pool;
    volatile bool m_abort;
    mutable PMutex m_mutex;
    PCondition m_condition;
};

TASKSPOOL_NS_END

#endif /* TASKTHREAD_H_ */
