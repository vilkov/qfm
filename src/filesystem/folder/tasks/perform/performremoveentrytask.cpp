#include "performremoveentrytask.h"
#include "../../../../application.h"
#ifdef Q_OS_WIN32
#	include <QtCore/qt_windows.h>
#endif
#include <QtCore/QDir>


FILE_SYSTEM_NS_BEGIN

PerformRemoveEntryTask::PerformRemoveEntryTask() :
	parent_class(new Params()),
	m_removeParentEntry(true),
	m_skipAllIfNotRemove(false),
	m_skipAllIfNotExists(false),
	m_progress(parameters()->source)
{}

PerformRemoveEntryTask::PerformRemoveEntryTask(Params *params) :
	parent_class(params),
	m_removeParentEntry(true),
	m_skipAllIfNotRemove(false),
	m_skipAllIfNotExists(false),
	m_progress(parameters()->source)
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

void PerformRemoveEntryTask::removeEntry(FolderNodeItem *entry, bool &tryAgain, const volatile bool &stopedFlag)
{
	if (entry->exists())
	{
		bool res;
		QString error;

		if (entry->isDir())
		{
			QDir dir = entry->absolutePath();
			res = dir.rmdir(entry->fileName());
		}
		else
		{
#			ifdef Q_OS_WIN32
				error = entry->absoluteFilePath();
				DWORD attr = GetFileAttributesW((const wchar_t*)error.utf16());
				if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
					SetFileAttributesW((const wchar_t*)error.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#			endif

			QFile file(entry->absoluteFilePath());

			if (file.open(QFile::ReadOnly))
			{
				m_progress.update(file.size());
				file.close();
			}

			res = file.remove();
			error = file.errorString();
		}

		if (!res && !m_skipAllIfNotRemove)
		{
			QuestionAnswerEvent::Params::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
			event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
			event->params().title = tr("Failed to remove");
			event->params().result = &result;

			if (entry->isDir())
				event->params().question = tr("Directory \"%1\". Skip it?").arg(entry->absoluteFilePath());
			else
				event->params().question = tr("File \"%1\" (%2). Skip it?").
					arg(entry->absoluteFilePath()).
					arg(error);

			Application::postEvent(parameters()->source.node, event.take());

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
			event->params().title = tr("Failed to remove");
			event->params().result = &result;

			if (entry->isDir())
				event->params().question = tr("Directory \"%1\" does not exists. Skip it?").arg(entry->absoluteFilePath());
			else
				event->params().question = tr("File \"%1\" does not exists. Skip it?").arg(entry->absoluteFilePath());

			Application::postEvent(parameters()->source.node, event.take());

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
	Application::postEvent(parameters()->source.node, event.take());
}

void PerformRemoveEntryTask::postCanceledEvent() const
{
	QScopedPointer<CanceledEvent> event(new CanceledEvent());
	event->params().snapshot = parameters()->source;
	event->params().removeParentEntry = m_removeParentEntry;
	Application::postEvent(parameters()->source.node, event.take());
}

FILE_SYSTEM_NS_END
