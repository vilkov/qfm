#include "performremovetask.h"
#include "../../../../application.h"
#ifdef Q_OS_WIN32
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QDir>


FILE_SYSTEM_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(QObject *receiver, QScopedPointer<FileSystemList> &entries) :
	parent_class(new Params(receiver, entries)),
	m_progress(receiver)
{}

void PerformRemoveTask::run(const volatile bool &stopedFlag)
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
		if ((entry = parameters()->entries->at(i))->isDir())
			m_progress.init(entry->fileName());

		removeEntry(entry, tryAgain = false, stopedFlag);
	}

	if (!stopedFlag && !isControllerDead())
		if (m_canceled)
			postCanceledEvent();
		else
			postCompletedEvent();
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
			QuestionAnswerEvent::Params::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
			event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
			event->params().title = tr("Failed to remove");
			event->params().result = &result;
			event->params().question = tr("Directory \"%1\". Skip it?").arg(entry->absoluteFilePath());

			Application::postEvent(parameters()->receiver, event.take());

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
			QuestionAnswerEvent::Params::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
			event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
			event->params().title = tr("Failed to remove");
			event->params().result = &result;
			event->params().question = tr("File \"%1\" (%2). Skip it?").arg(entry->absoluteFilePath()).arg(m_error);

			Application::postEvent(parameters()->receiver, event.take());

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

void PerformRemoveTask::postCompletedEvent() const
{
	QScopedPointer<CompletedEvent> event(new CompletedEvent());
	event->params().subnode.swap(parameters()->entries);
	Application::postEvent(parameters()->receiver, event.take());
}

void PerformRemoveTask::postCanceledEvent() const
{
	QScopedPointer<CanceledEvent> event(new CanceledEvent());
	event->params().subnode.swap(parameters()->entries);
	Application::postEvent(parameters()->receiver, event.take());
}

FILE_SYSTEM_NS_END
