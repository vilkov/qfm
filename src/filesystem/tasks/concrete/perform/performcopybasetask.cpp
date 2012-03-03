#include "performcopybasetask.h"
#include "../containers/filesysteminfolistitem.h"
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

PerformCopyBaseTask::PerformCopyBaseTask(TasksNode *receiver, const Snapshot &snapshot, IFileContainer::Holder &destination) :
	FilesBaseTask(receiver),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_doNotOverwriteAll(false),
	m_overwriteAll(false),
	m_progress(receiver),
	m_snapshot(snapshot),
	m_destination(destination.take())
{}

Snapshot PerformCopyBaseTask::copy(const volatile Flags &aborted)
{
	bool tryAgain;
	Snapshot::List list(m_snapshot);

	for (Snapshot::List::size_type i = 0; i < list.size() && !aborted; ++i)
	{
		m_progress.init(list.at(i).first);
		copyEntry(m_destination.data(), m_snapshot.container(), list.at(i).second, tryAgain = false, aborted);
		m_progress.complete();
	}

	return m_snapshot;
}

void PerformCopyBaseTask::copyEntry(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	do
		if (destination->contains(entry->fileName()))
			if (entry->isDir())
			{
				PScopedPointer<IFileContainer> dest;

				if (dest = destination->open(entry->fileName(), false, m_lastError))
					for (InfoListItem::size_type i = 0;
							i < static_cast<InfoListItem*>(entry)->size() && !aborted;
							++i)
						copyEntry(dest.data(), &static_cast<InfoListItem*>(entry)->container(), static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to open \"%1\". Skip it?").arg(m_lastError),
								tryAgain = false,
								aborted);
			}
			else
				if (m_overwriteAll)
					copyFile(destination, source, entry, tryAgain = false, aborted);
				else
					askForOverwrite(
							tr("Failed to copy..."),
							tr("File \"%1\" from \"%2\" already exists in \"%3\". Overwrite it?").
								arg(entry->fileName()).
								arg(entry->absolutePath()).
								arg(destination->location()),
							tryAgain = false,
							aborted);
		else
			if (entry->isDir())
			{
				PScopedPointer<IFileContainer> dest;

				if (dest = destination->open(entry->fileName(), true, m_lastError))
					for (InfoListItem::size_type i = 0;
							i < static_cast<InfoListItem*>(entry)->size() && !aborted;
							++i)
						copyEntry(dest.data(), &static_cast<InfoListItem*>(entry)->container(), static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to create \"%1\". Skip it?").arg(m_lastError),
								tryAgain = false,
								aborted);
			}
			else
				copyFile(destination, source, entry, tryAgain = false, aborted);
	while (tryAgain && !aborted);
}

void PerformCopyBaseTask::askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						title,
						text,
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::No |
						QMessageBox::NoToAll |
						QMessageBox::Cancel,
						aborted);

	switch (answer)
	{
		case QMessageBox::Yes:
			tryAgain = true;
			break;

		case QMessageBox::YesToAll:
			m_overwriteAll = tryAgain = true;
			break;

		case QMessageBox::No:
			tryAgain = false;
			break;

		case QMessageBox::NoToAll:
			m_overwriteAll = tryAgain = false;
			break;

		case QMessageBox::Cancel:
			cancel();
			break;
	}
}

void PerformCopyBaseTask::askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						title,
						text,
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::Retry |
						QMessageBox::Cancel,
						aborted);

	if (answer == QMessageBox::YesToAll)
		m_skipAllIfNotCopy = true;
	else
		if (answer == QMessageBox::Retry)
			tryAgain = true;
		else
			if (answer == QMessageBox::Cancel)
				cancel();
}

FILE_SYSTEM_NS_END
