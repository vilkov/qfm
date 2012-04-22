#include "filesystemfilesextendedbasetask.h"


FILE_SYSTEM_NS_BEGIN

FilesExtendedBaseTask::FilesExtendedBaseTask(TasksNode *receiver, ICopyControl::Holder &destination) :
	ExtendedBaseTask(receiver, destination)
{}

FILE_SYSTEM_NS_END
