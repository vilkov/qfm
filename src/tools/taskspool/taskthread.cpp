#include "taskthread.h"
#include "taskpool.h"
#include "../pointers/pscopedpointer.h"
#include "../exceptionshandler/exceptionshandler.h"


TASKSPOOL_NS_BEGIN

TaskThread::TaskThread(TaskPool *pool, Task *task) :
	PThread(),
	m_task(task),
	m_pool(pool)
{}

TaskThread::~TaskThread()
{
	terminate();
}

void TaskThread::handle(Task *task)
{
	PMutexLocker locker(m_mutex);
	m_task = task;
    m_condition.wakeOne();
}

void TaskThread::terminate()
{
	PMutexLocker locker(m_mutex);
	m_abort = true;
	m_condition.wakeOne();
	locker.unlock();
	wait();
    delete m_task;
}

void TaskThread::run()
{
	PMutexLocker locker(m_mutex);

    for (;;)
    {
        if (m_abort)
            return;
        else
			if (m_task == 0)
				m_condition.wait(m_mutex);
			else
			{
				PScopedPointer<Task> task(m_task);
				Task::Bit bit(m_abort, 0, task->flags());
				m_task = 0;

				locker.unlock();
				TRY
				{
					task->run(task->flags());
				}
				CATCH_ALL
				(
					"TaskThread::run",
					DO_NOTHING
				)
				locker.lock();

				m_task = m_pool->nextTask(this);
			}
    }
}

TASKSPOOL_NS_END
