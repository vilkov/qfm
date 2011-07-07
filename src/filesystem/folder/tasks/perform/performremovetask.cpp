#include "performremovetask.h"
#include "../../../../application.h"
#ifdef Q_OS_WIN32
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QDir>


FILE_SYSTEM_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(QObject *receiver, PScopedPointer<FileSystemList> &entries) :
	PerformTask(receiver),
	m_progress(receiver),
	m_entries(entries.take())
{}

void PerformRemoveTask::run(const volatile bool &stopedFlag)
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
		if ((entry = m_entries->at(i))->isDir())
		{
			m_progress.init(entry->fileName());
			removeEntry(entry, tryAgain = false, stopedFlag);
			m_progress.completed();
		}
		else
		{
			m_progress.clear();
			removeEntry(entry, tryAgain = false, stopedFlag);
		}
	}

	if (!stopedFlag && !isControllerDead())
	{
		PScopedPointer<Event> event(new Event(Event::RemoveFiles, m_entries, m_canceled));
		Application::postEvent(receiver(), event.take());
	}
}

void PerformRemoveTask::removeEntry(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	entry->refresh();

	if (entry->exists())
		if (entry->isDir())
		{
			FileSystemItem *localEntry;

			for (FileSystemList::size_type i = 0, size = static_cast<FileSystemList*>(entry)->size(); i < size; ++i)
			{
				removeEntry(localEntry = static_cast<FileSystemList*>(entry)->at(i), tryAgain = false, stopedFlag);

				if (!localEntry->shouldRemove())
					entry->setShouldRemove(false);
			}

			if (entry->shouldRemove())
				do
					removeDir(entry, tryAgain = false, stopedFlag);
				while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
		}
		else
			do
				removeFile(entry, tryAgain = false, stopedFlag);
			while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
}

void PerformRemoveTask::removeDir(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	QDir dir = entry->absolutePath();

	if (!dir.rmdir(entry->fileName()))
		if (m_skipAllIfNotRemove)
			entry->setShouldRemove(false);
		else
		{
			QuestionAnswerEvent::Result result;
			PScopedPointer<QuestionAnswerEvent> event(
					new QuestionAnswerEvent(
							tr("Failed to remove"),
							tr("Directory \"%1\". Skip it?").arg(entry->absoluteFilePath()),
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
				{
					m_skipAllIfNotRemove = true;
					entry->setShouldRemove(false);
				}
				else
					if (result.answer() == QMessageBox::Retry)
						tryAgain = true;
					else
					{
						if (result.answer() == QMessageBox::Cancel)
							m_canceled = true;

						entry->setShouldRemove(false);
					}
		}
}

void PerformRemoveTask::removeFile(FileSystemItem *entry, volatile bool &tryAgain, const volatile bool &stopedFlag)
{
	if (!doRemoveFile(entry->absoluteFilePath(), m_error))
		if (m_skipAllIfNotRemove)
			entry->setShouldRemove(false);
		else
		{
			QuestionAnswerEvent::Result result;
			PScopedPointer<QuestionAnswerEvent> event(
					new QuestionAnswerEvent(
							tr("Failed to remove"),
							tr("File \"%1\" (%2). Skip it?").arg(entry->absoluteFilePath()).arg(m_error),
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
				{
					m_skipAllIfNotRemove = true;
					entry->setShouldRemove(false);
				}
				else
					if (result.answer() == QMessageBox::Retry)
						tryAgain = true;
					else
					{
						if (result.answer() == QMessageBox::Cancel)
							m_canceled = true;

						entry->setShouldRemove(false);
					}
		}
}

bool PerformRemoveTask::doRemoveFile(const QString &filePath, QString &error)
{
#ifdef Q_OS_WIN32
	DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif

	QFile file(filePath);

	if (file.open(QFile::ReadOnly))
	{
		m_progress.update(file.size());
		file.close();

		if (file.remove())
			return true;
		else
			error = file.errorString();
	}
	else
		error = file.errorString();

	return false;
}

FILE_SYSTEM_NS_END
