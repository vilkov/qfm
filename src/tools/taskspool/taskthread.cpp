#include "taskthread.h"
#include "taskpool.h"
#include "../exceptionshandler/exceptionshandler.h"
#include <QtCore/QScopedPointer>


TASKSPOOL_NS_BEGIN

TaskThread::TaskThread(TaskPool *pool, Task *task, Priority priority) :
	QThread(0),
	m_task(task),
	m_pool(pool),
	m_abort(false)
{
    start(priority);
}

TaskThread::~TaskThread()
{
    QMutexLocker locker(&m_mutex);
    m_abort = true;
    m_condition.wakeOne();
    locker.unlock();
    wait();
    delete m_task;
}

void TaskThread::handle(Task *task)
{
	QMutexLocker locker(&m_mutex);
	m_task = task;
    m_condition.wakeOne();
}

void TaskThread::run()
{
	QMutexLocker locker(&m_mutex);

    forever
    {
        if (m_abort)
            return;
        else
			if (m_task == 0)
				m_condition.wait(&m_mutex);
			else
			{
				QScopedPointer<Task> task(m_task);
				m_task = 0;

				locker.unlock();
				TRY
				{
					task->run(m_abort);
				}
				CATCH_ALL
				(
					"TaskThread::run",
					DO_NOTHING
				)
				locker.relock();

				m_task = m_pool->nextTask(this);
			}
    }
}

TASKSPOOL_NS_END
