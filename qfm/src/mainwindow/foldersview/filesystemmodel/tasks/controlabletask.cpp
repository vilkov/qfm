#include "controlabletask.h"


ControlableTask::ControlableTask(Params *parameters) :
	m_params(parameters),
	m_controllerDead(false)
{
	Q_ASSERT(m_params);
}
