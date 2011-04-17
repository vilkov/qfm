#include "controlabletask.h"
#include <QtCore/QThread>


ControlableTask::ControlableTask(Params *parameters, QObject *controller1) :
	m_params(parameters),
	m_isControllerDead(false)
{
	Q_ASSERT(m_params);
	Q_ASSERT(controller1 != 0 && controller1->thread() == QThread::currentThread());

	memset(&m_handlers, 0, sizeof(DeleteHandler *) * MaxNumberOfControllers);
	m_handlers[0] = new DeleteHandler(0, this, controller1);
}

ControlableTask::ControlableTask(Params *parameters, QObject *controller1, QObject *controller2) :
	m_params(parameters),
	m_isControllerDead(false)
{
	Q_ASSERT(m_params);
	Q_ASSERT(controller1 != 0 && controller1->thread() == QThread::currentThread());
	Q_ASSERT(controller2 != 0 && controller2->thread() == QThread::currentThread());

	memset(&m_handlers, 0, sizeof(DeleteHandler *) * MaxNumberOfControllers);
	m_handlers[0] = new DeleteHandler(0, this, controller1);
	m_handlers[1] = new DeleteHandler(1, this, controller2);
}

ControlableTask::ControlableTask(Params *parameters, QObject *controller1, QObject *controller2, QObject *controller3) :
	m_params(parameters),
	m_isControllerDead(false)
{
	Q_ASSERT(m_params);
	Q_ASSERT(controller1 != 0 && controller1->thread() == QThread::currentThread());
	Q_ASSERT(controller2 != 0 && controller2->thread() == QThread::currentThread());
	Q_ASSERT(controller3 != 0 && controller3->thread() == QThread::currentThread());

	memset(&m_handlers, 0, sizeof(DeleteHandler *) * MaxNumberOfControllers);
	m_handlers[0] = new DeleteHandler(0, this, controller1);
	m_handlers[1] = new DeleteHandler(1, this, controller2);
	m_handlers[2] = new DeleteHandler(2, this, controller3);
}

ControlableTask::~ControlableTask()
{
	QMutexLocker locker(&m_params->m_mutex);

	for (size_type i = 0; i < MaxNumberOfControllers; ++i)
		if (m_handlers[i] != 0)
		{
			m_handlers[i]->m_task = 0;
			m_handlers[i]->deleteLater();
		}
}
