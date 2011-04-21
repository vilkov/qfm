#include "destcontrolabletask.h"
#include <QtCore/QThread>


DestControlableTask::DestControlableTask(Params *parameters) :
	ControlableTask(parameters),
	m_handler(new DeleteHandler(this, parameters->destination.object))
{
	Q_ASSERT(parameters->destination.object != 0);
	Q_ASSERT(parameters->destination.object->thread() == QThread::currentThread());
	Q_ASSERT(parameters->destination.fileSystemTree != 0);
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
