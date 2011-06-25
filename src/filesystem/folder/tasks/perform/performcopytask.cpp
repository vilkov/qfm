#include "performcopytask.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

PerformCopyTask::PerformCopyTask(QObject *receiver, QScopedPointer<FileSystemList> &entries, IFileControl *destination, bool move) :
	PerformTask(new Params(receiver, entries, destination, move), receiver),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_doNotOverwriteAll(false),
	m_overwriteAll(false),
	m_progress(receiver)
{}

void PerformCopyTask::run(const volatile bool &stopedFlag)
{
	parameters()->destination->refresh();

	if (parameters()->destination->exists())
	{
		bool tryAgain;
		FileSystemItem *entry;

		for (FileSystemList::size_type i = 0;
				i < parameters()->entries->size() &&
				!isControllerDead() &&
				!stopedFlag &&
				!m_canceled;
				++i)
		{
			m_progress.init((entry = parameters()->entries->at(i))->fileName());
			copyEntry(parameters()->destination, entry, tryAgain = false, stopedFlag);
		}
	}
	else
		m_canceled = true;

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event());
		event->params().entries.swap(parameters()->entries);
		event->params().canceled = m_canceled;
		event->params().move = parameters()->move;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->receiver, event.take());
	}
}

void PerformCopyTask::copyEntry(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	do
		if (destination->exists(entry))
			if (entry->isDir())
				if (IFileControl *dest = destination->open(entry->fileName(), m_lastError))
					for (FileSystemList::size_type i = 0;
							i < static_cast<FileSystemList*>(entry)->size() &&
							!isControllerDead() &&
							!stopedFlag &&
							!m_canceled;
							++i)
						copyEntry(dest, static_cast<FileSystemList*>(entry)->at(i), tryAgain = false, stopedFlag);
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
			else
				if (m_overwriteAll || tryAgain)
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
				if (IFileControl *dest = destination->open(entry->fileName(), m_lastError))
					for (FileSystemList::size_type i = 0;
							i < static_cast<FileSystemList*>(entry)->size() &&
							!isControllerDead() &&
							!stopedFlag &&
							!m_canceled;
							++i)
						copyEntry(dest, static_cast<FileSystemList*>(entry)->at(i), tryAgain = false, stopedFlag);
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
			else
				copyFile(destination, entry, tryAgain = false, stopedFlag);
	while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
}

void PerformCopyTask::copyFile(IFileControl *destination, FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	do
		if (IFile *sourceFile = entry->open(IFile::ReadOnly, m_lastError))
		{
			if (m_destEntry = destination->create(entry, m_lastError))
			{
				if (m_destFile = m_destEntry->open(IFile::WriteOnly, m_lastError))
				{
					m_written = 0;

					while (m_readed = sourceFile->read(m_buffer, FileReadWriteGranularity) &&
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

					m_destEntry->close(m_destFile);

					if (m_written == sourceFile->size())
					{
						destination->close(m_destEntry);
						entry->close(sourceFile);
						break;
					}
				}

				destination->close(m_destEntry);
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

			entry->close(sourceFile);
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
						stopedFlag);
	while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
}

void PerformCopyTask::askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	QuestionAnswerEvent::Params::Result result;
	QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
	event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll | QMessageBox::Cancel;
	event->params().title = title;
	event->params().result = &result;
	event->params().question = text;

	Application::postEvent(parameters()->receiver, event.take());

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
	QuestionAnswerEvent::Params::Result result;
	QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
	event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
	event->params().title = title;
	event->params().result = &result;
	event->params().question = text;

	Application::postEvent(parameters()->receiver, event.take());

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
