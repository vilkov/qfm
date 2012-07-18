#include "defaultperformmovetask.h"


DEFAULT_PLUGIN_NS_BEGIN

PerformMoveTask::PerformMoveTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot) :
	PerformCopyTask(receiver, destination, snapshot, true)
{}

void PerformMoveTask::copyFile(const IFileContainer *destination, SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (destination->move(entry->container(), entry->info(), m_lastError))
		m_progress.update(entry->info()->fileSize());
	else
		PerformCopyTask::copyFile(destination, entry, tryAgain = false, aborted);
}

DEFAULT_PLUGIN_NS_END
