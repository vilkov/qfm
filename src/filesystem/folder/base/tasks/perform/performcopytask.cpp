#include "performcopytask.h"
#include "../../../../tasks/containers/filesysteminfolistitem.h"
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

PerformCopyTask::PerformCopyTask(TasksNode *receiver, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control, bool move) :
	BaseTask(receiver),
	m_entries(entries),
	m_control(control.take()),
	m_move(move),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_doNotOverwriteAll(false),
	m_overwriteAll(false),
	m_progress(receiver)
{}

void PerformCopyTask::run(const volatile Flags &aborted)
{
	bool tryAgain;
	ScanedFiles::List list(m_entries);

	for (ScanedFiles::List::size_type i = 0; i < list.size() && !aborted; ++i)
	{
		m_progress.init(list.at(i).first);
		copyEntry(m_control.data(), list.at(i).second, tryAgain = false, aborted);
		m_progress.complete();
	}

	postEvent(new Event(this, aborted, m_entries, m_control, m_move));
}

void PerformCopyTask::copyEntry(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
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
						copyEntry(dest.data(), static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to open directory \"%1\". Skip it?").arg(m_lastError),
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
						copyEntry(dest.data(), static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to create directory \"%1\". Skip it?").arg(m_lastError),
								tryAgain = false,
								aborted);
			}
			else
				copyFile(destination, entry, tryAgain = false, aborted);
	while (tryAgain && !aborted);
}

void PerformCopyTask::copyFile(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	do
		if (m_sourceFile = entry->open(IFileAccessor::ReadOnly, m_lastError))
		{
			m_destEntry = destination->open(entry->fileName());

			if (m_destFile = m_destEntry->open(IFileAccessor::ReadWrite | IFileAccessor::Create | IFileAccessor::Truncate, m_lastError))
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
									arg(m_destEntry->absoluteFilePath()).
									arg(m_lastError),
								tryAgain = false,
								aborted);

						break;
					}

				if (m_written == m_sourceFile->size())
					break;
			}
		}
		else
			if (m_skipAllIfNotCopy || tryAgain)
				break;
			else
				askForSkipIfNotCopy(
						tr("Failed to copy..."),
						tr("Failed to open file \"%1\" (%2). Skip it?").
							arg(entry->absoluteFilePath()).
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

FILE_SYSTEM_NS_END
