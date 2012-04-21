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
