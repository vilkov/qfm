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
