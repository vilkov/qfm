#include "destcontrolabletask.h"
#include <QtCore/QThread>


FILE_SYSTEM_NS_BEGIN

DestControlableTask::DestControlableTask(Params *parameters) :
	ControlableTask(parameters),
	m_handler(new DeleteHandler(this, parameters->destination.node))
{
	Q_ASSERT(parameters->destination.node != 0);
	Q_ASSERT(parameters->destination.node->thread() == QThread::currentThread());
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
