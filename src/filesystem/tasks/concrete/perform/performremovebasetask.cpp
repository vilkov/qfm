#include "performremovebasetask.h"
#include "../containers/filesysteminfolistitem.h"

#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

PerformRemoveBaseTask::PerformRemoveBaseTask(TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_snapshot(snapshot),
	m_progress(receiver)
{}

Snapshot PerformRemoveBaseTask::remove(const volatile Flags &aborted)
{
	bool tryAgain;
	InfoItem *entry;
	Snapshot::List list(m_snapshot);

	for (Snapshot::List::size_type i = 0; i < list.size() && !aborted; ++i)
		if ((entry = list.at(i).second)->isDir())
		{
			m_progress.init(list.at(i).first);
			removeEntry(m_snapshot.container(), entry, tryAgain = false, aborted);
			m_progress.complete();
		}
		else
		{
			m_progress.clear();
			removeEntry(m_snapshot.container(), entry, tryAgain = false, aborted);
		}

	return m_snapshot;
}

void PerformRemoveBaseTask::removeEntry(const IFileContainer *root, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (entry->isDir())
	{
		InfoItem *localEntry;

		for (InfoListItem::size_type i = 0, size = static_cast<InfoListItem*>(entry)->size(); i < size; ++i)
		{
			removeEntry(&static_cast<InfoListItem*>(entry)->container(), localEntry = static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);

			if (!localEntry->isRemoved())
				entry->setRemoved(false);
		}

		if (entry->isRemoved())
			do
				doRemove(root, entry, tryAgain = false, aborted);
			while (tryAgain && !aborted);
	}
	else
		do
			doRemove(root, entry, tryAgain = false, aborted);
		while (tryAgain && !aborted);
}

void PerformRemoveBaseTask::doRemove(const IFileContainer *root, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (!root->remove(entry->fileName(), m_error))
		if (m_skipAllIfNotRemove)
			entry->setRemoved(false);
		else
		{
			qint32 answer = askUser(
								tr("Failed to remove"),
								tr("\"%1\" (%2). Skip it?").
									arg(root->location(entry->fileName())).
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
