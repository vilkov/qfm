#include "defaultperformmovetask.h"


DEFAULT_PLUGIN_NS_BEGIN

DefaultPerformMoveTask::DefaultPerformMoveTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot) :
	DefaultPerformCopyTask(receiver, destination, snapshot, true)
{}

void DefaultPerformMoveTask::copyFile(const IFileContainer *destination, WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (destination->move(entry->container(), entry->info()->fileName(), m_lastError))
		m_progress.update(entry->info()->fileSize());
	else
		DefaultPerformCopyTask::copyFile(destination, entry, tryAgain, aborted);
}

DEFAULT_PLUGIN_NS_END
