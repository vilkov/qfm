#include "filesystembasetask.h"
#include <QtCore/QThread>


FILE_SYSTEM_NS_BEGIN

class BaseTask::DeleteHandler : public QObject
{
public:
	DeleteHandler(BaseTask *task, QObject *parent) :
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
	 * Just holds the reference to BaseTask::m_mutexHolder,
	 * so it can not be deleted before this object.
	 *
	 */
	MutexHolderPointer mutexHolder;
	BaseTask *task;
};


BaseTask::BaseTask(QObject *receiver) :
	m_mutexHolder(new MutexHolder()),
	m_receiver(receiver),
	m_handler(new DeleteHandler(this, m_receiver)),
	m_canceled(false),
	m_controllerDead(false)
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

BaseTask::~BaseTask()
{
	PMutexLocker locker(m_mutexHolder->mutex);

	if (m_handler != 0)
	{
		m_handler->task = 0;
		m_handler->deleteLater();
	}
}

FILE_SYSTEM_NS_END
