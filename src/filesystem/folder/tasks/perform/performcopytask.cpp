#include "performcopytask.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

PerformCopyTask::PerformCopyTask(QObject *receiver, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move) :
	PerformTask(receiver),
	m_entries(entries.take()),
	m_destination(destination.take()),
	m_move(move),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_doNotOverwriteAll(false),
	m_overwriteAll(false),
	m_progress(receiver)
{}

void PerformCopyTask::run(const volatile bool &stopedFlag)
{
	m_destination->refresh();

	if (m_destination->exists())
	{
		bool tryAgain;
		FileSystemItem *entry;

		for (FileSystemList::size_type i = 0;
				i < m_entries->size() &&
				!isControllerDead() &&
				!stopedFlag &&
				!m_canceled;
				++i)
		{
			m_progress.init((entry = m_entries->at(i))->fileName());
			copyEntry(m_destination.data(), entry, tryAgain = false, stopedFlag);
			m_progress.completed();
		}
	}
	else
		m_canceled = true;

	if (!stopedFlag && !isControllerDead())
	{
		PScopedPointer<Event> event(new Event(m_entries, m_canceled, m_destination, m_move));
		Application::postEvent(receiver(), event.take());
	}
}

void PerformCopyTask::copyEntry(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	do
		if (destination->contains(entry))
			if (entry->isDir())
			{
				PScopedPointer<IFileControl> dest;

				if (dest = destination->open(entry, m_lastError))
					for (FileSystemList::size_type i = 0;
							i < static_cast<FileSystemList*>(entry)->size() &&
							!isControllerDead() &&
							!stopedFlag &&
							!m_canceled;
							++i)
						copyEntry(dest.data(), static_cast<FileSystemList*>(entry)->at(i), tryAgain = false, stopedFlag);
				else
					if (m_skipAllIfNotCopy || tryAgain)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to open directory \"%1\". Skip it?").
									arg(destination->absoluteFilePath(entry->fileName())),
								tryAgain = false,
								stopedFlag);
			}
			else
				if (m_overwriteAll)
					copyFile(destination, entry, tryAgain = false, stopedFlag);
				else
					askForOverwrite(
							tr("Failed to copy..."),
							tr("File \"%1\" from \"%2\" already exists in \"%3\". Overwrite it?").
								arg(entry->fileName()).
								arg(entry->absolutePath()).
								arg(destination->absoluteFilePath()),
							tryAgain = false,
							stopedFlag);
		else
			if (entry->isDir())
			{
				PScopedPointer<IFileControl> dest;

				if (dest = destination->create(entry, m_lastError))
					for (FileSystemList::size_type i = 0;
							i < static_cast<FileSystemList*>(entry)->size() &&
							!isControllerDead() &&
							!stopedFlag &&
							!m_canceled;
							++i)
						copyEntry(dest.data(), static_cast<FileSystemList*>(entry)->at(i), tryAgain = false, stopedFlag);
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to create directory \"%1\". Skip it?").
									arg(destination->absoluteFilePath(entry->fileName())),
								tryAgain = false,
								stopedFlag);
			}
			else
				copyFile(destination, entry, tryAgain = false, stopedFlag);
	while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
}

void PerformCopyTask::copyFile(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	do
		if (m_sourceFile = entry->open(IFile::ReadOnly, m_lastError))
			if (m_destEntry = destination->create(entry, m_lastError))
			{
				if (m_destFile = m_destEntry->open(IFile::WriteOnly, m_lastError))
				{
					m_written = 0;

					while ((m_readed = m_sourceFile->read(m_buffer, FileReadWriteGranularity)) &&
							!isControllerDead() && !stopedFlag && !m_canceled)
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
									stopedFlag);

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
							tr("Failed to create file \"%1\" (%2). Skip it?").
								arg(destination->absoluteFilePath(entry->fileName())).
								arg(m_lastError),
							tryAgain = false,
							stopedFlag);
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
						stopedFlag);
	while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
}

void PerformCopyTask::askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	QuestionAnswerEvent::Result result;
	PScopedPointer<QuestionAnswerEvent> event(
			new QuestionAnswerEvent(
					title,
					text,
					QMessageBox::Yes |
					QMessageBox::YesToAll |
					QMessageBox::No |
					QMessageBox::NoToAll |
					QMessageBox::Cancel,
					&result
			)
	);

	Application::postEvent(receiver(), event.take());

	if (result.waitFor(stopedFlag, isControllerDead()))
		switch (result.answer())
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
				m_canceled = true;
				break;
		}
}

void PerformCopyTask::askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	QuestionAnswerEvent::Result result;
	PScopedPointer<QuestionAnswerEvent> event(
			new QuestionAnswerEvent(
					title,
					text,
					QMessageBox::Yes |
					QMessageBox::YesToAll |
					QMessageBox::Retry |
					QMessageBox::Cancel,
					&result
			)
	);

	Application::postEvent(receiver(), event.take());

	if (result.waitFor(stopedFlag, isControllerDead()))
		if (result.answer() == QMessageBox::YesToAll)
			m_skipAllIfNotCopy = true;
		else
			if (result.answer() == QMessageBox::Retry)
				tryAgain = true;
			else
				if (result.answer() == QMessageBox::Cancel)
					m_canceled = true;
}

FILE_SYSTEM_NS_END
