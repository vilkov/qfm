#include "taskpool.h"
#include "../pointers/pscopedpointer.h"


template <typename T>
struct Destroy
{
	void operator()(T data) { delete data; }
};

template <typename Container>
void deleteAll(Container &container)
{
	for (typename Container::iterator it = container.begin(), end = container.end(); it != end; ++it)
		delete *it;
}


TASKSPOOL_NS_BEGIN

TaskPool::TaskPool(types::int32_t maxThreads) :
	m_clearing(false),
	m_maxThreads(maxThreads)
{}

TaskPool::~TaskPool()
{
	clear();
}

void TaskPool::handle(Task *task)
{
	PMutexLocker locker(m_mutex);

	if (!m_clearing)
		if (m_freeThreads.empty())
			if (m_threads.size() < m_maxThreads)
			{
				PScopedPointer<TaskThread> thread(new TaskThread(this, task));
				m_threads.push_back(thread.data());
				thread.take()->start();
			}
			else
				m_tasks.push_back(task);
		else
		{
			m_freeThreads.front()->handle(task);
			m_freeThreads.pop_front();
		}
}

void TaskPool::clear()
{
	m_mutex.lock();
	m_clearing = true;
	m_mutex.unlock();

	deleteAll(m_tasks);
	m_tasks.clear();

	deleteAll(m_threads);
	m_threads.clear();
	m_freeThreads.clear();

	m_mutex.lock();
	m_clearing = false;
	m_mutex.unlock();
}

Task *TaskPool::nextTask(TaskThread *thread)
{
	PMutexLocker locker(m_mutex);

	if (!m_clearing)
		if (m_tasks.empty())
			m_freeThreads.push_back(thread);
		else
		{
			Task *res = m_tasks.front();
			m_tasks.pop_front();
			return res;
		}

	return 0;
}

TASKSPOOL_NS_END
