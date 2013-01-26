/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "taskthread.h"
#include "taskpool.h"
#include "../memory/memory_scopedpointer.h"
#include "../exceptionshandler/exceptionshandler.h"


TASKSPOOL_NS_BEGIN

TaskThread::TaskThread(TaskPool *pool, Task *task) :
	Thread(),
	m_task(task),
	m_pool(pool)
{
	start();
}

TaskThread::~TaskThread()
{
	terminate();
}

void TaskThread::handle(Task *task)
{
	Mutex::Locker locker(m_mutex);
	m_task = task;
    m_condition.wakeOne();
}

void TaskThread::terminate()
{
	Mutex::Locker locker(m_mutex);
	m_abort = true;
	m_condition.wakeOne();
	locker.unlock();
	wait();
    delete m_task;
}

void TaskThread::run()
{
	Mutex::Locker lock(m_mutex);

    for (;;)
    {
        if (m_abort)
            return;
        else
			if (m_task == 0)
				m_condition.wait(m_mutex);
			else
			{
				::Tools::Memory::ScopedPointer<Task> task(m_task);
				Task::Bit bit(m_abort, 0, task->flags());
				m_task = 0;

				TRY
				{
				    Mutex::Unlocker unlock(lock);
					task->run(task->flags());
				}
				CATCH_ALL
				(
					"TaskThread::run",
					DO_NOTHING
				)

				m_task = m_pool->nextTask(this);
			}
    }
}

TASKSPOOL_NS_END
