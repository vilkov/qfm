#include "performcopytask.h"
#include <QtCore/QDir>


PerformCopyTask::PerformCopyTask(Params *params) :
	PerformTask(params, params->receiver)
{
	Q_ASSERT(params->entry != 0);
	Q_ASSERT(params->subtree != 0);
	Q_ASSERT(params->destination != 0);
	Q_ASSERT(params->destinationDirectory.isEmpty());
}

void PerformCopyTask::run(const volatile bool &stopedFlag)
{
	QDir dir(parameters()->destinationDirectory);

	if (dir.exists())
	{

	}
}
