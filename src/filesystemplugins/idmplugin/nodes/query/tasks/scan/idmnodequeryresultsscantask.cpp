#include "idmnodequeryresultsscantask.h"


IDM_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(TasksNode *receiver, const TasksNode::TasksItemList &files) :
	ScanFilesBaseTask(receiver),
	m_files(files)
{}

void ScanFilesTask::run(const volatile bool &aborted)
{

}

IDM_PLUGIN_NS_END
