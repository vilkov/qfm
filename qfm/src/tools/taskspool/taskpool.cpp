#include "taskpool.h"
#include <QtCore/QScopedPointer>


TASKSPOOL_NS_BEGIN

TaskPool::TaskPool(qint32 maxThreads) :
	m_clearing(false),
	m_maxThreads(maxThreads)
{}

TaskPool::~TaskPool()
{
	clear();
}

void TaskPool::handle(Task *task)
{
	QMutexLocker locker(&m_mutex);

	if (!m_clearing)
		if (m_freeThreads.isEmpty())
			if (m_threads.size() < m_maxThreads)
				m_threads.push_back(new TaskThread(this, task));
			else
				m_tasks.enqueue(task);
		else
			m_freeThreads.takeFirst()->handle(task);
}

void TaskPool::clear()
{
	m_mutex.lock();
	m_clearing = true;
	m_mutex.unlock();

	qDeleteAll(m_tasks);
	m_tasks.clear();

	qDeleteAll(m_threads);
	m_threads.clear();
	m_freeThreads.clear();

	m_mutex.lock();
	m_clearing = false;
	m_mutex.unlock();
}

Task *TaskPool::nextTask(TaskThread *thread)
{
	QMutexLocker locker(&m_mutex);

	if (!m_clearing)
		if (m_tasks.isEmpty())
			m_freeThreads.push_back(thread);
		else
			return m_tasks.dequeue();

	return 0;
}

TASKSPOOL_NS_END
