#include "idmnodequeryresultsscantask.h"


IDM_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(TasksNode *receiver, const List &files) :
	ScanFilesBaseTask(receiver),
	m_files(files)
{}

void ScanFilesTask::run(const volatile bool &aborted)
{

}

IDM_PLUGIN_NS_END
