#include "defaultperformremovetask.h"
#include "../../events/defaultmodelevent.h"

#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_snapshot(snapshot),
	m_skipAllIfNotRemove(false),
	m_progress(receiver)
{}

void PerformRemoveTask::run(const volatile Flags &aborted)
{
	bool tryAgain;
	WrappedNodeItem *entry;
	const Snapshot::List list = m_snapshot.list();

	for (Snapshot::List::const_iterator it = list.begin(), end = list.end(); it != end; ++it)
	{
		entry = (*it).second;

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
	}

	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::RemoveFiles), aborted, m_snapshot));
}

void PerformRemoveTask::removeEntry(WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (entry->info()->isDir())
	{
		WrappedNodeItem *localEntry;

		for (WrappedNodeItem::size_type i = 0, size = entry->size(); i < size; ++i)
		{
			removeEntry(localEntry = entry->at(i), tryAgain = false, aborted);

			if (!localEntry->isRemoved())
				entry->setRemoved(false);
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

void PerformRemoveTask::doRemove(WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (!entry->container()->remove(entry->info()->fileName(), m_error))
		if (m_skipAllIfNotRemove)
			entry->setRemoved(false);
		else
		{
			qint32 answer = askUser(
								tr("Failed to remove"),
								tr("\"%1\" (%2). Skip it?").
									arg(entry->container()->location(entry->info()->fileName())).
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
