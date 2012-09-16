#include "default_performremovetask.h"
#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_snapshot(snapshot),
	m_progress(receiver),
	m_skipAllIfNotRemove(false)
{}

void PerformRemoveTask::run(const volatile Flags &aborted)
{
	bool tryAgain;
	SnapshotItem *entry;

	for (Snapshot::iterator it = m_snapshot.begin(), end = m_snapshot.end(); it != end && !aborted; ++it)
		if ((entry = (*it).second)->info()->isDir())
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

	postEvent(new Event(this, Event::RemoveFiles, aborted, m_snapshot));
}

void PerformRemoveTask::removeEntry(SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (entry->info()->isDir())
	{
		SnapshotItem *localEntry;

		for (SnapshotItem::iterator i = entry->begin(), end = entry->end(); i != end && !aborted;)
		{
			removeEntry(localEntry = (*i), tryAgain = false, aborted);

			if (localEntry->isRemoved())
				i = entry->erase(i);
			else
			{
				++i;
				entry->setRemoved(false);
			}
		}

		if (entry->isRemoved())
			do
				doRemove(entry, tryAgain = false, aborted);
			while (tryAgain && !aborted);
	}
	else
		do
			doRemove(entry, tryAgain = false, aborted);
		while (tryAgain && !aborted);
}

void PerformRemoveTask::doRemove(SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (!entry->container()->remove(entry->info(), m_error))
		if (m_skipAllIfNotRemove)
			entry->setRemoved(false);
		else
		{
			qint32 answer = askUser(
								tr("Failed to remove"),
								tr("\"%1\" (%2). Skip it?").
									arg(entry->container()->location(entry->info())).
									arg(m_error),
								QMessageBox::Yes |
								QMessageBox::YesToAll |
								QMessageBox::Retry |
								QMessageBox::Cancel,
								aborted);

			if (answer == QMessageBox::YesToAll)
			{
				m_skipAllIfNotRemove = true;
				entry->setRemoved(false);
			}
			else
				if (answer == QMessageBox::Retry)
					tryAgain = true;
				else
				{
					if (answer == QMessageBox::Cancel)
						cancel();

					entry->setRemoved(false);
				}
		}
}

DEFAULT_PLUGIN_NS_END
