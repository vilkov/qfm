#include "performcopytask.h"
#include "../../../../application.h"
#include <QtCore/QFile>


PerformCopyTask::PerformCopyTask(Params *params) :
	PerformTask(params, params->receiver),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_overwriteAll(false),
	m_canceled(false)
{
	Q_ASSERT(params->entry != 0);
	Q_ASSERT(params->subtree != 0);
	Q_ASSERT(params->destination != 0);
	Q_ASSERT(params->destinationDirectory.isEmpty());
}

void PerformCopyTask::run(const volatile bool &stopedFlag)
{
	QDir dir(parameters()->destinationDirectory);

	if (dir.exists())
		copy(dir, parameters()->fileSystemTree, stopedFlag);
}

void PerformCopyTask::copy(QDir &destination, FileSystemTree *tree, const volatile bool &stopedFlag)
{
	bool tryAgain;
	QByteArray buffer;
	FileSystemEntry *entry;

	for (FileSystemTree::size_type i = 1, size = tree->size(); i < size && !stopedFlag && !m_canceled; ++i)
		if ((entry = static_cast<FileSystemEntry*>(tree->child(i)))->fileInfo().isDir())
			if (destination.cd(entry->fileInfo().fileName()))
			{

			}
			else
				if (destination.mkdir(entry->fileInfo().fileName()))
				{

				}
				else
				{
					QuestionAnswerParams::Result result;
					QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
					event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
					event->params().title = entry->lockReason();
					event->params().result = &result;
					event->params().question = tr("Failed to create directory \"%1\". Skip it?").
							arg(entry->fileInfo().fileName()).
							arg(entry->fileInfo().absolutePath()).
							arg(destination.absolutePath());

					Application::postEvent(parameters()->receiver, event.take());

					if (result.waitFor(stopedFlag))
						if (result.answer() == QMessageBox::YesToAll)
							m_skipAllIfNotCreate = true;
						else
							if (result.answer() == QMessageBox::Retry)
								--i;
							else
								if (result.answer() == QMessageBox::Cancel)
									m_canceled = true;
				}
		else
		{
			if (destination.exists(entry->fileInfo().fileName()) && !m_overwriteAll)
			{
				QuestionAnswerParams::Result result;
				QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
				event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Cancel;
				event->params().title = entry->lockReason();
				event->params().result = &result;
				event->params().question = tr("File \"%1\" from \"%2\" already exists in \"%3\". Overwrite it?").
						arg(entry->fileInfo().fileName()).
						arg(entry->fileInfo().absolutePath()).
						arg(destination.absolutePath());

				Application::postEvent(parameters()->receiver, event.take());

				if (result.waitFor(stopedFlag))
					if (result.answer() == QMessageBox::YesToAll)
						m_overwriteAll = true;
					else
						if (result.answer() == QMessageBox::Cancel)
						{
							m_canceled = true;
							break;
						}
			}

			QFile file(entry->fileInfo().absoluteFilePath());

			if (file.open(QFile::ReadOnly))
			{
				QFile dest(destination.absoluteFilePath(entry->fileInfo().fileName()));

				if (dest.open(QFile::WriteOnly | QFile::Truncate))
				{
					while (!file.atEnd() && !stopedFlag && !m_canceled)
						if (dest.write(buffer = file.read(ReadFileBufferSize)) != buffer.size())
						{
							QuestionAnswerParams::Result result;
							QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
							event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
							event->params().title = entry->lockReason();
							event->params().result = &result;
							event->params().question = tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
									arg(dest.errorString()).
									arg(entry->fileInfo().fileName()).
									arg(entry->fileInfo().absolutePath()).
									arg(destination.absolutePath());

							Application::postEvent(parameters()->receiver, event.take());

							if (result.waitFor(stopedFlag))
								if (result.answer() == QMessageBox::YesToAll)
									m_skipAllIfNotCopy = true;
								else
									if (result.answer() == QMessageBox::Retry)
										--i;
									else
										if (result.answer() == QMessageBox::Cancel)
											m_canceled = true;
						}
				}
				else
				{
					QuestionAnswerParams::Result result;
					QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
					event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
					event->params().title = entry->lockReason();
					event->params().result = &result;
					event->params().question = tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
							arg(dest.errorString()).
							arg(entry->fileInfo().fileName()).
							arg(entry->fileInfo().absolutePath()).
							arg(destination.absolutePath());

					Application::postEvent(parameters()->receiver, event.take());

					if (result.waitFor(stopedFlag))
						if (result.answer() == QMessageBox::YesToAll)
							m_skipAllIfNotCopy = true;
						else
							if (result.answer() == QMessageBox::Retry)
								--i;
							else
								if (result.answer() == QMessageBox::Cancel)
									m_canceled = true;
				}
			}
			else
			{
				tryAgain = false;
				askForSkipAllIfNotCopy(
						entry->lockReason(),
						tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
							arg(file.errorString()).
							arg(entry->fileInfo().fileName()).
							arg(entry->fileInfo().absolutePath()).
							arg(destination.absolutePath()),
						tryAgain,
						stopedFlag);

				if (tryAgain)
					--i;
			}
		}
}

void PerformCopyTask::askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag)
{
	QuestionAnswerParams::Result result;
	QScopedPointer<QuestionAnswerEvent> event(new QuestionAnswerEvent(QuestionAnswerEvent::QuestionAnswer));
	event->params().buttons = QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Retry | QMessageBox::Cancel;
	event->params().title = title;
	event->params().result = &result;
	event->params().question = text;

	Application::postEvent(parameters()->receiver, event.take());

	if (result.waitFor(stopedFlag))
		if (result.answer() == QMessageBox::YesToAll)
			m_skipAllIfNotCopy = true;
		else
			if (result.answer() == QMessageBox::Retry)
				tryAgain = true;
			else
				if (result.answer() == QMessageBox::Cancel)
					m_canceled = true;
}
