#include "idmnodequeryresultsperformremovetask.h"


IDM_PLUGIN_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot) :
	::VFS::Plugins::Default::PerformRemoveTask(receiver, snapshot)
{}

void PerformRemoveTask::run(const volatile Flags &aborted)
{
	bool tryAgain;
	SnapshotItem *entry;

	for (Snapshot::iterator it = m_snapshot.begin(), end = m_snapshot.end(); it != end && !aborted; ++it)
		if (entry = (*it).second)
			if (entry->info()->isDir())
			{
				m_progress.init((*it).first);
				removeEntry(entry, tryAgain = false, aborted);
				m_progress.complete();
			}
			else
			{
				m_progress.clear();
				removeEntry(entry, tryAgain = false, aborted);
			}

	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::RemoveFiles), aborted, m_snapshot));
}

IDM_PLUGIN_NS_END
