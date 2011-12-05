#include "idmnodequeryresultsremovefilestask.h"


IDM_PLUGIN_NS_BEGIN

RemoveFilesTask::RemoveFilesTask(TasksNode *receiver, const ScanedFiles &files) :
	PerformRemoveBaseTask(receiver, static_cast<PerformRemoveBaseTask::Event::Type>(ModelEvent::RemoveFiles), files)
{}

IDM_PLUGIN_NS_END
