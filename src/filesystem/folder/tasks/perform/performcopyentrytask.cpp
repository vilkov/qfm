#include "performcopyentrytask.h"
#include "../../../../application.h"


#ifdef Q_OS_WIN
#	include "routines/copyprogressroutine_win.h"
#else
#	include "routines/copyprogressroutine_default.h"
#endif


FILE_SYSTEM_NS_BEGIN

PerformCopyEntryTask::PerformCopyEntryTask(Params *params) :
	parent_class(params),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_overwriteAll(false),
	m_progress(params->source)
{
	Q_ASSERT(params->destination.node != 0);
}

void PerformCopyEntryTask::run(const volatile bool &stopedFlag)
{
	m_progress.init();

	QDir dir(parameters()->destination.node->absoluteFilePath());

	if (dir.exists())
	{
		bool tryAgain;

		do
			copyFile(dir, parameters()->source.entry, tryAgain = false, stopedFlag);
		while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);
	}
	else
		m_canceled = true;

	if (!stopedFlag && !isControllerDead())
		if (m_canceled)
		{
			QScopedPointer<CanceledEvent> event(new CanceledEvent());
			event->params().snapshot = parameters()->source;
			event->params().removeSource = parameters()->removeSource;
			event->params().destination = parameters()->destination;
			Application::postEvent(parameters()->source.node, event.take());
		}
		else
		{
			QScopedPointer<CompletedEvent> event(new CompletedEvent());
			event->params().snapshot = parameters()->source;
			event->params().removeSource = parameters()->removeSource;
			event->params().destination = parameters()->destination;
			Application::postEvent(parameters()->source.node, event.take());
		}
}

void PerformCopyEntryTask::copyFile(const QDir &destination, FolderNodeEntry *entry, bool &tryAgain, const volatile bool &stopedFlag)
{
	QFile file(entry->absoluteFilePath());
	QFile dest(destination.absoluteFilePath(entry->fileName()));

	if (dest.exists())
		if (!m_overwriteAll)
		{
			QuestionAnswerEvent::Params::Result result;
			QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
			event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Ignore | QMessageBox::Cancel;
			event->params().title = entry->lockReason();
			event->params().result = &result;
			event->params().question = tr("File \"%1\" from \"%2\" already exists in \"%3\". Overwrite it?").
					arg(entry->fileName()).
					arg(entry->absolutePath()).
					arg(destination.absolutePath());

			Application::postEvent(parameters()->source.node, event.take());

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

	CopyProgressRoutine routine(this, stopedFlag);

	if (!routine.copy(file, dest) && !m_skipAllIfNotCopy)
		askForSkipAllIfNotCopy(
				entry->lockReason(),
				tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
					arg(entry->fileName()).
					arg(routine.lastError()).
					arg(entry->absolutePath()).
					arg(destination.absolutePath()),
				tryAgain,
				stopedFlag);
}

void PerformCopyEntryTask::askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag)
{
	QuestionAnswerEvent::Params::Result result;
	QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent());
	event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
	event->params().title = title;
	event->params().result = &result;
	event->params().question = text;

	Application::postEvent(parameters()->source.node, event.take());

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
