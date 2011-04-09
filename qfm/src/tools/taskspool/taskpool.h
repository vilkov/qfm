#ifndef POOL_H_
#define POOL_H_

#include <QtCore/QList>
#include <QtCore/QQueue>
#include <QtCore/QMutex>
#include "ns_taskspool.h"
#include "task.h"
#include "taskthread.h"


TASKSPOOL_NS_BEGIN

class TaskPool
{
public:
	TaskPool(qint32 maxThreads);
	~TaskPool();

	void handle(Task *task);
	void clear();

protected:
	friend class TaskThread;
	Task *nextTask(TaskThread *thread);

private:
    QMutex m_mutex;
    bool m_clearing;
	qint32 m_maxThreads;
    QQueue<Task*> m_tasks;
	QList<TaskThread*> m_threads;
	QList<TaskThread*> m_freeThreads;
};

TASKSPOOL_NS_END

#endif /* POOL_H_ */
