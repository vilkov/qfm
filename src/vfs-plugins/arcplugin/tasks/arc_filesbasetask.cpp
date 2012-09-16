#include "arc_filesbasetask.h"


ARC_PLUGIN_NS_BEGIN

FilesBaseTask::FilesBaseTask(TasksNode *receiver) :
	Base(receiver)
{}

FilesBaseTask::FilesBaseTask(TasksNode *receiver, ICopyControl::Holder &destination) :
	Base(receiver, destination)
{}

ARC_PLUGIN_NS_END
