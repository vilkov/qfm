#include "arcreadarchivetask.h"


ARC_PLUGIN_NS_BEGIN

ReadArchiveTask::ReadArchiveTask(const QString &fileName, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName)
{}

void ReadArchiveTask::run(const volatile bool &aborted)
{

}

ARC_PLUGIN_NS_END
