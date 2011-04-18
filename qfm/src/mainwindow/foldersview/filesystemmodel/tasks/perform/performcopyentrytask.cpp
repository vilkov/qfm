#include "performcopyentrytask.h"
#include "../../../../../application.h"
#include <QtCore/QFile>


PerformCopyEntryTask::PerformCopyEntryTask(Params *params) :
	PerformTask(params, params->source.object),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_overwriteAll(false),
	m_canceled(false)
{
	Q_ASSERT(params->destination.object != 0);
	Q_ASSERT(params->destination.fileSystemTree != 0);
}

void PerformCopyEntryTask::run(const volatile bool &stopedFlag)
{
	Event::EventType type;
	QDir dir(parameters()->destination.fileSystemTree->fileInfo().absoluteFilePath());

	if (dir.exists())
	{
		bool tryAgain;

		do
			copyFile(dir, parameters()->source.entry, tryAgain = false, stopedFlag);
		while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);

		type = m_canceled ? Event::CopyFilesCanceled : Event::CopyFilesComplete;
	}
	else
		type = Event::CopyFilesCanceled;

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(type));
		event->params().snapshot.fileSystemTree = parameters()->source.fileSystemTree;
		event->params().snapshot.entry = parameters()->source.entry;
		Application::postEvent(parameters()->source.object, event.take());
	}
}

void PerformCopyEntryTask::copyFile(const QDir &destination, FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag)
{
	QFile file(entry->fileInfo().absoluteFilePath());
	QFile dest(destination.absoluteFilePath(entry->fileInfo().fileName()));

	if (dest.exists())
		if (m_overwriteAll)
		{
			if (!dest.remove())
			{
				if (m_skipAllIfNotCopy)
					return;
				else
					askForSkipAllIfNotCopy(
							entry->lockReason(),
							tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
								arg(entry->fileInfo().fileName()).
								arg(file.errorString()).
								arg(entry->fileInfo().absolutePath()).
								arg(destination.absolutePath()),
							tryAgain,
							stopedFlag);
			}
		}
		else
		{
			QuestionAnswerParams::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
			event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Ignore | QMessageBox::Cancel;
			event->params().title = entry->lockReason();
			event->params().result = &result;
			event->params().question = tr("File \"%1\" from \"%2\" already exists in \"%3\". Overwrite it?").
					arg(entry->fileInfo().fileName()).
					arg(entry->fileInfo().absolutePath()).
					arg(destination.absolutePath());

			Application::postEvent(parameters()->source.object, event.take());

			if (result.waitFor(stopedFlag, isControllerDead()))
				if (result.answer() == QMessageBox::YesToAll)
					m_overwriteAll = true;
				else
					if (result.answer() == QMessageBox::Cancel)
					{
						m_canceled = true;
						return;
					}
					else
						if (result.answer() == QMessageBox::Ignore)
							return;
		}

	if (!file.copy(dest.fileName()))
		if (!m_skipAllIfNotCopy)
			askForSkipAllIfNotCopy(
					entry->lockReason(),
					tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
						arg(entry->fileInfo().fileName()).
						arg(file.errorString()).
						arg(entry->fileInfo().absolutePath()).
						arg(destination.absolutePath()),
					tryAgain,
					stopedFlag);
}

void PerformCopyEntryTask::askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag)
{
	QuestionAnswerParams::Result result;
	QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
	event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
	event->params().title = title;
	event->params().result = &result;
	event->params().question = text;

	Application::postEvent(parameters()->source.object, event.take());

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
