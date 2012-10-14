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
#ifndef TASKTHREAD_H_
#define TASKTHREAD_H_

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
    virtual ~TaskThread();

	virtual void terminate();
    void handle(Task *task);

protected:
    virtual void run();

private:
    Task *m_task;
    TaskPool *m_pool;
    Task::Flag m_abort;
    PMutex m_mutex;
    PCondition m_condition;
};

TASKSPOOL_NS_END

#endif /* TASKTHREAD_H_ */
