#ifndef PMUTEX_QT_H_
#define PMUTEX_QT_H_

#include <QtCore/QMutex>


class PMutex
{
public:
	PMutex()
	{}

	void lock() { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }

private:
	friend class PCondition;
	friend class PMutexLocker;

private:
	QMutex m_mutex;
};


class PMutexLocker
{
public:
	explicit PMutexLocker(PMutex &mutex) :
		m_locker(&mutex.m_mutex)
	{}

	void lock() { m_locker.relock(); }
	void unlock() { m_locker.unlock(); }

private:
	QMutexLocker m_locker;
};

#endif /* PMUTEX_QT_H_ */
