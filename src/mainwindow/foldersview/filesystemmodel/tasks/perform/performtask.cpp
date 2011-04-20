#include "performtask.h"


PerformTask::PerformTask(Params *params, QObject *controller1) :
	ControlableTask(params, controller1),
	m_canceled(false)
{
	Q_ASSERT(params->source.object);
	Q_ASSERT(params->source.fileSystemTree);
	Q_ASSERT(params->source.entry);
}

PerformTask::PerformTask(Params *params, QObject *controller1, QObject *controller2) :
	ControlableTask(params, controller1, controller2),
	m_canceled(false)
{
	Q_ASSERT(params->source.object);
	Q_ASSERT(params->source.fileSystemTree);
	Q_ASSERT(params->source.entry);
}

PerformTask::PerformTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3) :
	ControlableTask(params, controller1, controller2, controller3),
	m_canceled(false)
{
	Q_ASSERT(params->source.object);
	Q_ASSERT(params->source.fileSystemTree);
	Q_ASSERT(params->source.entry);
}