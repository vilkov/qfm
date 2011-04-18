#include "performremoveentrytask.h"
#include "../../../../../application.h"
#ifdef Q_OS_WIN32
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QDir>


PerformRemoveEntryTask::PerformRemoveEntryTask(Params *params) :
	PerformTask(params, params->source.object),
	m_shoulRemoveEntry(true),
	m_skipAllIfNotRemove(false),
	m_skipAllIfNotExists(false)
{}

void PerformRemoveEntryTask::run(const volatile bool &stopedFlag)
{
	bool tryAgain;

	do
		removeEntry(parameters()->source.entry, tryAgain = false, stopedFlag);
	while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(m_canceled ? Event::RemoveFilesCanceled : Event::RemoveFilesComplete));
		event->params().snapshot.fileSystemTree = parameters()->source.fileSystemTree;
		event->params().snapshot.entry = parameters()->source.entry;
		event->params().shoulRemoveEntry = m_shoulRemoveEntry;
		Application::postEvent(parameters()->source.object, event.take());
	}
}

void PerformRemoveEntryTask::removeEntry(FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag)
{
	if (entry->fileInfo().exists())
	{
		bool res;
		QString error;

		if (entry->fileInfo().isDir())
		{
			QDir dir = entry->fileInfo().absoluteDir();
			res = dir.rmdir(entry->fileInfo().fileName());
		}
		else
		{
#			ifdef Q_OS_WIN32
				DWORD attr = GetFileAttributesW((const wchar_t*)entry->fileInfo().absoluteFilePath().data());
				if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
					SetFileAttributesW((const wchar_t*)entry->fileInfo().absoluteFilePath().data(), attr &= ~FILE_ATTRIBUTE_READONLY);
#			endif

			QFile file(entry->fileInfo().absoluteFilePath());
			res = file.remove();
			error = file.errorString();
		}

		if (!res && !m_skipAllIfNotRemove)
		{
			QuestionAnswerParams::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
			event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
			event->params().title = entry->lockReason();
			event->params().result = &result;

			if (entry->fileInfo().isDir())
				event->params().question = tr("Failed to remove directory \"%1\". Skip it?").arg(entry->fileInfo().absoluteFilePath());
			else
				event->params().question = tr("Failed to remove file \"%1\" (%2). Skip it?").
					arg(entry->fileInfo().absoluteFilePath()).
					arg(error);

			Application::postEvent(parameters()->source.object, event.take());

			if (result.waitFor(stopedFlag, isControllerDead()))
				if (result.answer() == QMessageBox::YesToAll)
				{
					m_shoulRemoveEntry = false;
					m_skipAllIfNotRemove = true;
				}
				else
					if (result.answer() == QMessageBox::Retry)
						tryAgain = true;
					else
						if (result.answer() == QMessageBox::Cancel)
							m_canceled = true;
						else
							m_shoulRemoveEntry = false;
		}
	}
	else
		if (!m_skipAllIfNotExists)
		{
			QuestionAnswerParams::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
			event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Cancel;
			event->params().title = entry->lockReason();
			event->params().result = &result;

			if (entry->fileInfo().isDir())
				event->params().question = tr("Directory \"%1\" does not exists. Skip it?").arg(entry->fileInfo().absoluteFilePath());
			else
				event->params().question = tr("File \"%1\" does not exists. Skip it?").arg(entry->fileInfo().absoluteFilePath());

			Application::postEvent(parameters()->source.object, event.take());

			if (result.waitFor(stopedFlag, isControllerDead()))
				if (result.answer() == QMessageBox::YesToAll)
					m_skipAllIfNotExists = true;
				else
					if (result.answer() == QMessageBox::Cancel)
						m_canceled = true;
		}
}
