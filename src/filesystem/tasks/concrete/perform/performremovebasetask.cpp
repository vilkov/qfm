#include "performremovebasetask.h"
#include "../containers/filesysteminfolistitem.h"

#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

PerformRemoveBaseTask::PerformRemoveBaseTask(TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_snapshot(snapshot),
	m_skipAllIfNotRemove(false),
	m_progress(receiver)
{}

Snapshot PerformRemoveBaseTask::remove(const volatile Flags &aborted)
{
	bool tryAgain;
	InfoItem *entry;
	Snapshot::List list(m_snapshot);

	for (Snapshot::List::size_type i = 0; i < list.size() && !aborted; ++i)
	{
		entry = list.at(i).second;

		if (entry->container()->contains(entry->fileName()))
			if (entry->isDir())
			{
				m_progress.init(list.at(i).first);
				removeEntry(entry, tryAgain = false, aborted);
				m_progress.complete();
			}
			else
			{
				m_progress.clear();
				removeEntry(entry, tryAgain = false, aborted);
			}
	}

	return m_snapshot;
}

void PerformRemoveBaseTask::removeEntry(InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (entry->isDir())
	{
		InfoItem *localEntry;

		for (InfoListItem::size_type i = 0, size = static_cast<InfoListItem*>(entry)->size(); i < size; ++i)
		{
			localEntry = static_cast<InfoListItem*>(entry)->at(i);

			if (localEntry->container()->contains(localEntry->fileName()))
			{
				removeEntry(localEntry, tryAgain = false, aborted);

				if (!localEntry->isRemoved())
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

void PerformRemoveBaseTask::doRemove(InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (!entry->container()->remove(entry->fileName(), m_error))
		if (m_skipAllIfNotRemove)
			entry->setRemoved(false);
		else
		{
			qint32 answer = askUser(
								tr("Failed to remove"),
								tr("\"%1\" (%2). Skip it?").
									arg(entry->container()->location(entry->fileName())).
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

FILE_SYSTEM_NS_END
