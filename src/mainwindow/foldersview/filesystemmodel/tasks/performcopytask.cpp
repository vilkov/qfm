#include "performcopytask.h"
#include "../../../../application.h"
#include <QtCore/QFile>


PerformCopyTask::PerformCopyTask(Params *params) :
	PerformTask(params, params->source.object),
	m_skipAllIfNotCreate(false),
	m_skipAllIfNotCopy(false),
	m_overwriteAll(false),
	m_canceled(false)
{
	Q_ASSERT(params->source.entry != 0);
	Q_ASSERT(params->subtree != 0);
	Q_ASSERT(params->destination.object != 0);
	Q_ASSERT(params->destination.fileSystemTree != 0);
}

void PerformCopyTask::run(const volatile bool &stopedFlag)
{
	Event::EventType type;
	QDir dir(parameters()->destination.fileSystemTree->fileInfo().absoluteFilePath());

	if (dir.exists() && cd(dir, parameters()->source.entry, stopedFlag))
	{
		copy(dir, parameters()->subtree, stopedFlag);
		type = m_canceled ? Event::CopyFilesCanceled : Event::CopyFilesComplete;
	}
	else
		type = Event::CopyFilesCanceled;

//	QScopedPointer<Event> event(new Event(type));
//	event->params().fileSystemTree = parameters()->fileSystemTree;
//	event->params().size = parameters()->size;
//	event->params().entry = parameters()->entry;
//	event->params().subtree = parameters()->subtree;
//	event->params().destination = parameters()->destination;
//	event->params().destinationDirectory = parameters()->destinationDirectory;
//	Application::postEvent(parameters()->receiver, event.take());
}

bool PerformCopyTask::cd(QDir &destination, FileSystemEntry *entry, const volatile bool &stopedFlag)
{
	if (destination.cd(entry->fileInfo().fileName()))
		return true;
	else
	{
		bool tryAgain;

		do
			if (destination.mkdir(entry->fileInfo().fileName()))
				if (destination.cd(entry->fileInfo().fileName()))
					return true;
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipAllIfNotCopy(
								entry->lockReason(),
								tr("Failed to create directory \"%1\" in \"%2\". Skip it?").
									arg(entry->fileInfo().fileName()).
									arg(destination.absolutePath()),
								tryAgain = false,
								stopedFlag);
			else
				if (m_skipAllIfNotCopy)
					break;
				else
					askForSkipAllIfNotCopy(
							entry->lockReason(),
							tr("Failed to create directory \"%1\" in \"%2\". Skip it?").
								arg(entry->fileInfo().fileName()).
								arg(destination.absolutePath()),
							tryAgain = false,
							stopedFlag);
		while (tryAgain && !stopedFlag && !m_canceled);
	}

	return false;
}

void PerformCopyTask::copy(QDir &destination, FileSystemTree *tree, const volatile bool &stopedFlag)
{
	FileSystemEntry *entry;

	for (FileSystemTree::size_type i = 1, size = tree->size(); i < size && !stopedFlag && !m_canceled; ++i)
		if ((entry = static_cast<FileSystemEntry*>(tree->child(i)))->fileInfo().isDir())
		{
			if (cd(destination, entry, stopedFlag))
			{
				copy(destination, static_cast<FileSystemTree*>(tree->subtree(entry)), stopedFlag);
				destination.cdUp();
			}
		}
		else
		{
			if (destination.exists(entry->fileInfo().fileName()) && !m_overwriteAll)
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
							break;
						}
						else
							if (result.answer() == QMessageBox::Ignore)
								continue;
			}

			copyFile(destination, entry, stopedFlag);
		}
}

void PerformCopyTask::copyFile(QDir &destination, FileSystemEntry *entry, const volatile bool &stopedFlag)
{
	bool ok;
	bool tryAgain;
	QByteArray buffer;

	do
	{
		QFile file(entry->fileInfo().absoluteFilePath());

		if (file.open(QFile::ReadOnly))
		{
			QFile dest(destination.absoluteFilePath(entry->fileInfo().fileName()));

			if (dest.open(QFile::WriteOnly | QFile::Truncate))
			{
				ok = true;

				while (!file.atEnd())
					if (stopedFlag || m_canceled)
					{
						dest.close();
						dest.remove();
					}
					else
						if (dest.write(buffer = file.read(ReadFileBufferSize)) != buffer.size())
							if (m_skipAllIfNotCopy)
							{
								dest.close();
								dest.remove();
							}
							else
							{
								ok = false;

								askForSkipAllIfNotCopy(
										entry->lockReason(),
										tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
											arg(file.errorString()).
											arg(entry->fileInfo().fileName()).
											arg(entry->fileInfo().absolutePath()).
											arg(destination.absolutePath()),
										tryAgain = false,
										stopedFlag);
							}

				if (ok)
					break;
			}
			else
				if (m_skipAllIfNotCopy)
					break;
				else
					askForSkipAllIfNotCopy(
							entry->lockReason(),
							tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
								arg(file.errorString()).
								arg(entry->fileInfo().fileName()).
								arg(entry->fileInfo().absolutePath()).
								arg(destination.absolutePath()),
							tryAgain = false,
							stopedFlag);
		}
		else
			if (m_skipAllIfNotCopy)
				break;
			else
				askForSkipAllIfNotCopy(
						entry->lockReason(),
						tr("Failed to copy file \"%1\" (%2) from \"%3\" to \"%4\". Skip it?").
							arg(file.errorString()).
							arg(entry->fileInfo().fileName()).
							arg(entry->fileInfo().absolutePath()).
							arg(destination.absolutePath()),
						tryAgain = false,
						stopedFlag);
	}
	while (tryAgain && !stopedFlag && !m_canceled);
}

void PerformCopyTask::askForSkipAllIfNotCopy(const QString &title, const QString &text, bool &tryAgain, const volatile bool &stopedFlag)
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
