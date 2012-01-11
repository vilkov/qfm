#include "arcperformcopytask.h"


ARC_PLUGIN_NS_BEGIN

PerformCopyTask::PerformCopyTask(const QString &fileName, TasksNode *receiver) :
	BaseTask(receiver),
	m_fileName(fileName)
{}

void PerformCopyTask::run(const volatile bool &aborted)
{

}

ARC_PLUGIN_NS_END
