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
#include "taskpool.h"
#include <algorithm>


template <typename T>
struct Destroy
{
	void operator()(T data) { delete data; }
};

template <typename Container>
void deleteAll(Container &container)
{
	std::for_each(container.begin(), container.end(), Destroy<typename Container::value_type>());
}


TASKSPOOL_NS_BEGIN

TaskPool::TaskPool(int32_t maxThreads) :
	m_maxThreads(maxThreads)
{}

TaskPool::~TaskPool()
{
	clear();
	deleteAll(m_threads);
}

bool TaskPool::handleImmediately(Task *task)
{
	PMutexLocker locker(m_mutex);

	if (m_freeThreads.empty())
		if (m_threads.size() < m_maxThreads)
			m_threads.push_back(new TaskThread(this, task));
		else
			return false;
	else
	{
		m_freeThreads.front()->handle(task);
		m_freeThreads.pop_front();
	}

	return true;
}

void TaskPool::handle(Task *task)
{
	PMutexLocker locker(m_mutex);

	if (m_freeThreads.empty())
		if (m_threads.size() < m_maxThreads)
			m_threads.push_back(new TaskThread(this, task));
		else
			m_tasks.push_back(task);
	else
	{
		m_freeThreads.front()->handle(task);
		m_freeThreads.pop_front();
	}
}

bool TaskPool::remove(Task *task)
{
	PMutexLocker locker(m_mutex);

	for (std::list<Task*>::iterator it = m_tasks.begin(), end = m_tasks.end(); it != end; ++it)
		if ((*it) == task)
		{
			m_tasks.erase(it);
			return true;
		}

	return false;
}

void TaskPool::clear()
{
	PMutexLocker locker(m_mutex);
	m_tasks.clear();
}

Task *TaskPool::nextTask(TaskThread *thread)
{
	PMutexLocker locker(m_mutex);

	if (m_tasks.empty())
		m_freeThreads.push_back(thread);
	else
	{
		Task *res = m_tasks.front();
		m_tasks.pop_front();
		return res;
	}

	return NULL;
}

TASKSPOOL_NS_END
