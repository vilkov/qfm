#include "performremovetask.h"


FILE_SYSTEM_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const ScanedFiles &files) :
	PerformRemoveBaseTask(receiver, static_cast<PerformRemoveBaseTask::Event::Type>(ModelEvent::RemoveFiles), files)
{}

FILE_SYSTEM_NS_END
