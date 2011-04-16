#include "performtask.h"


PerformTask::PerformTask(Params *params, QObject *controller1) :
	ControlableTask(params, controller1)
{
	Q_ASSERT(params->fileSystemTree);
	Q_ASSERT(params->receiver);
}

PerformTask::PerformTask(Params *params, QObject *controller1, QObject *controller2) :
	ControlableTask(params, controller1, controller2)
{
	Q_ASSERT(params->fileSystemTree);
	Q_ASSERT(params->receiver);
}

PerformTask::PerformTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3) :
	ControlableTask(params, controller1, controller2, controller3)
{
	Q_ASSERT(params->fileSystemTree);
	Q_ASSERT(params->receiver);
}
