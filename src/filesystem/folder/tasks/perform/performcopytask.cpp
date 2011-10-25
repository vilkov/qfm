#include "performcopytask.h"
#include "../../../../application.h"


FILE_SYSTEM_NS_BEGIN

PerformCopyTask::PerformCopyTask(QObject *receiver, PScopedPointer<InfoListItem> &entries, PScopedPointer<ICopyControl> &control, bool move) :
	PerformTask(receiver),
	m_entries(entries.take()),
	m_control(control.take()),
	m_move(move),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_doNotOverwriteAll(false),
	m_overwriteAll(false),
	m_progress(receiver)
{}

void PerformCopyTask::run(const volatile bool &aborted)
{
	m_control->refresh();

	if (m_control->exists())
	{
		bool tryAgain;
		InfoItem *entry;

		for (InfoListItem::size_type i = 0;
				i < m_entries->size() &&
				!isCanceled() &&
				!isControllerDead() &&
				!aborted;
				++i)
		{
			m_progress.init((entry = m_entries->at(i))->fileName());
			copyEntry(m_control.data(), entry, tryAgain = false, aborted);
			m_progress.complete();
		}
	}
	else
		cancel();

	if (!aborted && !isControllerDead())
	{
		PScopedPointer<Event> event(new Event(isCanceled(), m_entries, m_control, m_move));
		Application::postEvent(receiver(), event.take());
	}
}

void PerformCopyTask::copyEntry(IFileControl *destination, InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted)
{
	do
		if (destination->contains(entry))
			if (entry->isDir())
			{
				PScopedPointer<IFileControl> dest;

				if (dest = destination->open(entry, m_lastError))
					for (InfoListItem::size_type i = 0;
							i < static_cast<InfoListItem*>(entry)->size() &&
							!isCanceled() &&
							!isControllerDead() &&
							!aborted;
							++i)
						copyEntry(dest.data(), static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);
				else
					if (m_skipAllIfNotCopy || tryAgain)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to open directory \"%1\". Skip it?").
									arg(destination->absoluteFilePath(entry->fileName())),
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
								arg(destination->absoluteFilePath()),
							tryAgain = false,
							aborted);
		else
			if (entry->isDir())
			{
				PScopedPointer<IFileControl> dest;

				if (dest = destination->create(entry, m_lastError))
					for (InfoListItem::size_type i = 0;
							i < static_cast<InfoListItem*>(entry)->size() &&
							!isCanceled() &&
							!isControllerDead() &&
							!aborted;
							++i)
						copyEntry(dest.data(), static_cast<InfoListItem*>(entry)->at(i), tryAgain = false, aborted);
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Failed to copy..."),
								tr("Failed to create directory \"%1\". Skip it?").
									arg(destination->absoluteFilePath(entry->fileName())),
								tryAgain = false,
								aborted);
			}
			else
				copyFile(destination, entry, tryAgain = false, aborted);
	while (tryAgain && !isCanceled() && !isControllerDead() && !aborted);
}

void PerformCopyTask::copyFile(IFileControl *destination, InfoItem *entry, volatile bool &tryAgain, const volatile bool &aborted)
{
	do
		if (m_sourceFile = entry->open(IFile::ReadOnly, m_lastError))
			if (m_destEntry = destination->create(entry, m_lastError))
			{
				if (m_destFile = m_destEntry->open(IFile::WriteOnly, m_lastError))
				{
					m_written = 0;

					while ((m_readed = m_sourceFile->read(m_buffer, FileReadWriteGranularity)) &&
							!isCanceled() && !isControllerDead() && !aborted)
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
							tr("Failed to create file \"%1\" (%2). Skip it?").
								arg(destination->absoluteFilePath(entry->fileName())).
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
							arg(entry->absoluteFilePath()).
							arg(m_lastError),
						tryAgain = false,
						aborted);
	while (tryAgain && !isCanceled() && !isControllerDead() && !aborted);
}

void PerformCopyTask::askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &aborted)
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

	if (result.waitFor(aborted, isControllerDead()))
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
				cancel();
				break;
		}
}

void PerformCopyTask::askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile bool &aborted)
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

	if (result.waitFor(aborted, isControllerDead()))
		if (result.answer() == QMessageBox::YesToAll)
			m_skipAllIfNotCopy = true;
		else
			if (result.answer() == QMessageBox::Retry)
				tryAgain = true;
			else
				if (result.answer() == QMessageBox::Cancel)
					cancel();
}

FILE_SYSTEM_NS_END
