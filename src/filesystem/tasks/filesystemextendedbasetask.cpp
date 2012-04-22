#include "filesystemextendedbasetask.h"


FILE_SYSTEM_NS_BEGIN

ExtendedBaseTask::ExtendedBaseTask(TasksNode *receiver, ICopyControl::Holder &destination) :
	BaseTask(receiver),
    m_destination(destination.take())
{}

FILE_SYSTEM_NS_END
