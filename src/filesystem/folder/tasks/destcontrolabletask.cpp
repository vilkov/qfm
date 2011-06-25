#include "destcontrolabletask.h"
#include <QtCore/QThread>


FILE_SYSTEM_NS_BEGIN

DestControlableTask::DestControlableTask(Params *parameters, QObject *listener) :
	ControlableTask(parameters),
	m_listener(listener),
	m_handler(new DeleteHandler(this, listener))
{
	Q_ASSERT(m_listener != 0);
	Q_ASSERT(m_listener->thread() == QThread::currentThread());
}

DestControlableTask::~DestControlableTask()
{
	QMutexLocker locker(&parameters()->m_mutex);

	if (m_handler != 0)
	{
		m_handler->m_task = 0;
		m_handler->deleteLater();
	}
}

FILE_SYSTEM_NS_END
