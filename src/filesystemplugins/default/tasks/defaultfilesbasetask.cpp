#include "defaultfilesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

FilesBaseTask::FilesBaseTask(TasksNode *receiver) :
	BaseTask(receiver)
{}

FilesBaseTask::FilesBaseTask(TasksNode *receiver, ICopyControl::Holder &destination) :
	BaseTask(receiver, destination)
{}

DEFAULT_PLUGIN_NS_END
