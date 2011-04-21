#include "performcopytreetask.h"
#include "../../../../../application.h"
#include <QtCore/QFile>


PerformCopyTreeTask::PerformCopyTreeTask(Params *params) :
	PerformCopyEntryTask(params)
{
	Q_ASSERT(params->subtree != 0);
}

void PerformCopyTreeTask::run(const volatile bool &stopedFlag)
{
	Event::EventType type;
	QDir dir(parameters()->destination.fileSystemTree->fileInfo().absoluteFilePath());

	if (dir.exists() && cd(dir, parameters()->source.entry, stopedFlag))
	{
		copy(dir, parameters()->subtree, stopedFlag);
		dir.cdUp();
		type = m_canceled ? Event::CopyFilesCanceled : Event::CopyFilesComplete;
	}
	else
		type = Event::CopyFilesCanceled;

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(type));
		event->params().snapshot = parameters()->source;
		event->params().removeSource = parameters()->removeSource;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->source.object, event.take());
	}
}

bool PerformCopyTreeTask::cd(QDir &destination, FileSystemEntry *entry, const volatile bool &stopedFlag)
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
		while (tryAgain && !stopedFlag && !isControllerDead() && !m_canceled);
	}

	return false;
}

void PerformCopyTreeTask::copy(QDir &destination, FileSystemTree *tree, const volatile bool &stopedFlag)
{
	bool tryAgain;
	FileSystemEntry *entry;

	for (FileSystemTree::size_type i = 1, size = tree->size(); i < size && !stopedFlag && !isControllerDead() && !m_canceled; ++i)
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
			copyFile(destination, entry, tryAgain = false, stopedFlag);

			if (tryAgain)
				--i;
		}
}
