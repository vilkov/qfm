#ifndef TASKTHREAD_H_
#define TASKTHREAD_H_

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include "ns_taskspool.h"
#include "task.h"


TASKSPOOL_NS_BEGIN

class TaskPool;

class TaskThread : public QThread
{
public:
    TaskThread(TaskPool *pool, Task *task, Priority priority = LowPriority);
    ~TaskThread();

    void handle(Task *task);
    void terminate();

protected:
    virtual void run();

private:
    Task *m_task;
    TaskPool *m_pool;
    volatile bool m_abort;
    mutable QMutex m_mutex;
    QWaitCondition m_condition;
};

TASKSPOOL_NS_END

#endif /* TASKTHREAD_H_ */
