#include "destcontrolabletask.h"
#include <QtCore/QThread>


FILE_SYSTEM_NS_BEGIN

DestControlableTask::DestControlableTask(QObject *receiver) :
    m_mutexHolder(new MutexHolder()),
	m_receiver(receiver),
	m_handler(new DeleteHandler(this, receiver))
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

DestControlableTask::~DestControlableTask()
{
	QMutexLocker locker(&m_mutexHolder->mutex);

	if (m_handler != 0)
	{
		m_handler->task = 0;
		m_handler->deleteLater();
	}
}

FILE_SYSTEM_NS_END
