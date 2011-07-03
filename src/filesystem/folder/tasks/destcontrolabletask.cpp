#include "destcontrolabletask.h"
#include <QtCore/QThread>


FILE_SYSTEM_NS_BEGIN

class DestControlableTask::DeleteHandler : public QObject
{
public:
	DeleteHandler(DestControlableTask *task, QObject *parent) :
		QObject(parent),
		mutexHolder(task->m_mutexHolder),
		task(task)
	{}
	virtual ~DeleteHandler()
	{
		PMutexLocker locker(mutexHolder->mutex);

		if (task != 0)
		{
			task->m_handler = 0;
			task->m_receiver = 0;
			task->m_controllerDead = true;
		}
	}

	/*
	 * Just holds the reference to DestControlableTask::m_mutexHolder,
	 * so it can not be deleted before this object.
	 */
	MutexHolderPointer mutexHolder;
	DestControlableTask *task;
};


DestControlableTask::DestControlableTask(QObject *receiver) :
    m_mutexHolder(new MutexHolder()),
	m_receiver(receiver),
	m_handler(new DeleteHandler(this, receiver)),
	m_controllerDead(false)
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

DestControlableTask::~DestControlableTask()
{
	PMutexLocker locker(m_mutexHolder->mutex);

	if (m_handler != 0)
	{
		m_handler->task = 0;
		m_handler->deleteLater();
	}
}

FILE_SYSTEM_NS_END
