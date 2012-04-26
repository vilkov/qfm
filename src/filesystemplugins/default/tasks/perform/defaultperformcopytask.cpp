#include "defaultperformcopytask.h"
#include "../../events/defaultmodelevent.h"

#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

PerformCopyTask::PerformCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move) :
	FilesExtendedBaseTask(receiver, destination),
	m_progress(receiver),
	m_snapshot(snapshot),
	m_move(move),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_doNotOverwriteAll(false),
	m_overwriteAll(false)
{}

void PerformCopyTask::run(const volatile Flags &aborted)
{
	bool tryAgain;
	const Snapshot::List list = m_snapshot.list();

	for (Snapshot::List::const_iterator it = list.begin(), end = list.end(); it != end; ++it)
	{
		m_progress.init((*it).first);
		copyEntry(destination().data(), (*it).second, tryAgain = false, aborted);
		m_progress.complete();
	}

	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::CopyFiles), destination(), aborted, m_snapshot, m_move));
}

void PerformCopyTask::copyEntry(const IFileContainer *destination, WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	do
		if (destination->contains(entry->info()->fileName()))
			if (entry->info()->isDir())
			{
				PScopedPointer<IFileContainer> dest;

				if (dest = destination->open(entry->info()->fileName(), false, m_lastError))
					for (WrappedNodeItem::size_type i = 0; i < entry->size() && !aborted; ++i)
						copyEntry(dest.data(), entry->at(i), tryAgain = false, aborted);
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
					copyFile(destination, entry, tryAgain = false, aborted);
				else
					askForOverwrite(
							tr("Failed to copy..."),
							tr("File \"%1\" from \"%2\" already exists in \"%3\". Overwrite it?").
								arg(entry->info()->fileName()).
								arg(entry->container()->location()).
								arg(destination->location()),
							tryAgain = false,
							aborted);
		else
			if (entry->info()->isDir())
			{
				PScopedPointer<IFileContainer> dest;

				if (dest = destination->open(entry->info()->fileName(), true, m_lastError))
					for (WrappedNodeItem::size_type i = 0; i < entry->size() && !aborted; ++i)
						copyEntry(dest.data(), entry->at(i), tryAgain = false, aborted);
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
				copyFile(destination, entry, tryAgain = false, aborted);
	while (tryAgain && !aborted);
}

void PerformCopyTask::copyFile(const IFileContainer *destination, WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	do
		if (m_sourceFile = entry->container()->open(entry->info()->fileName(), IFileAccessor::ReadOnly, m_lastError))
			if (m_destFile = destination->open(entry->info()->fileName(), IFileAccessor::ReadWrite | IFileAccessor::Create | IFileAccessor::Truncate, m_lastError))
			{
				m_written = 0;

				while ((m_readed = m_sourceFile->read(m_buffer, FileReadWriteGranularity)) && !aborted)
					if (m_destFile->write(m_buffer, m_readed) == m_readed)
					{
						m_written += m_readed;
						m_progress.update(m_readed);
					}
					else
					{
						m_lastError = m_destFile->lastError();

						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to write to file \"%1\" (%2). Skip it?").
									arg(destination->location(entry->info()->fileName())).
									arg(m_lastError),
								tryAgain = false,
								aborted);

						break;
					}

				if (m_written == m_sourceFile->size())
					break;
			}
			else
				if (m_skipAllIfNotCopy || tryAgain)
					break;
				else
					askForSkipIfNotCopy(
							tr("Failed to copy..."),
							tr("Failed to create file \"%1\" (%2). Skip it?").
								arg(destination->location(entry->info()->fileName())).
								arg(m_lastError),
							tryAgain = false,
							aborted);
		else
			if (m_skipAllIfNotCopy || tryAgain)
				break;
			else
				askForSkipIfNotCopy(
						tr("Failed to copy..."),
						tr("Failed to open file \"%1\" (%2). Skip it?").
							arg(entry->container()->location(entry->info()->fileName())).
							arg(m_lastError),
						tryAgain = false,
						aborted);
	while (tryAgain && !aborted);
}

void PerformCopyTask::askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
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

void PerformCopyTask::askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
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

DEFAULT_PLUGIN_NS_END
