#include "performremoveentrytask.h"
#include "../../../../../application.h"
#ifdef Q_OS_WIN32
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QDir>


PerformRemoveEntryTask::PerformRemoveEntryTask(Params *params) :
	parent_class(params),
	m_removeParentEntry(true),
	m_skipAllIfNotRemove(false),
	m_skipAllIfNotExists(false),
	m_progress(params->source)
{}

void PerformRemoveEntryTask::run(const volatile bool &stopedFlag)
{
	bool tryAgain;

	m_progress.init();

	do
		removeEntry(parameters()->source.entry, tryAgain = false, stopedFlag);
	while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);

	if (!stopedFlag && !isControllerDead())
		if (m_canceled)
			postCanceledEvent();
		else
			postCompletedEvent();
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
				error = entry->fileInfo().absoluteFilePath();
				DWORD attr = GetFileAttributesW((const wchar_t*)error.utf16());
				if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
					SetFileAttributesW((const wchar_t*)error.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#			endif

			QFile file(entry->fileInfo().absoluteFilePath());

			res = file.remove();
			error = file.errorString();
			m_progress.update(file.size());
		}

		if (!res && !m_skipAllIfNotRemove)
		{
			QuestionAnswerEvent::Params::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
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
					m_removeParentEntry = false;
					m_skipAllIfNotRemove = true;
				}
				else
					if (result.answer() == QMessageBox::Retry)
						tryAgain = true;
					else
						if (result.answer() == QMessageBox::Cancel)
							m_canceled = true;
						else
							m_removeParentEntry = false;
		}
	}
	else
		if (!m_skipAllIfNotExists)
		{
			QuestionAnswerEvent::Params::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
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

void PerformRemoveEntryTask::postCompletedEvent() const
{
	QScopedPointer<CompletedEvent> event(new CompletedEvent());
	event->params().snapshot = parameters()->source;
	event->params().removeParentEntry = m_removeParentEntry;
	Application::postEvent(parameters()->source.object, event.take());
}

void PerformRemoveEntryTask::postCanceledEvent() const
{
	QScopedPointer<CanceledEvent> event(new CanceledEvent());
	event->params().snapshot = parameters()->source;
	event->params().removeParentEntry = m_removeParentEntry;
	Application::postEvent(parameters()->source.object, event.take());
}
