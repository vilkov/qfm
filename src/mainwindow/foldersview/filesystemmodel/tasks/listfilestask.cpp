#include "listfilestask.h"
#include "../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>
#include <QScopedPointer>


ListFilesTask::ListFilesTask(Params *parameters) :
	FilesTask(parameters)
{}

void ListFilesTask::run(const volatile bool &stopedFlag)
{
	QTime base = QTime::currentTime();
	QTime current;
	QList<FileSystemInfo> updatedFiles;
	QDirIterator dirIt(parameters()->fileSystemTree->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while (!stopedFlag && !isReceiverDead() && dirIt.hasNext())
	{
		dirIt.next();
		current = QTime::currentTime();
		updatedFiles.push_back(getInfo(dirIt.fileInfo()));

		if (base.msecsTo(current) > 300)
		{
			QScopedPointer<Event> event(new Event(Event::ListFiles));
			event->params().fileSystemTree = parameters()->fileSystemTree;
			event->params().isLastEvent = false;
			event->params().updates = updatedFiles;
			Application::postEvent(parameters()->receiver, event.take());

			updatedFiles.clear();
			base = current;
		}
	}

	if (!stopedFlag && !isReceiverDead())
		if (updatedFiles.isEmpty())
		{
			QScopedPointer<Event> event(new Event(Event::ListFiles));
			event->params().fileSystemTree = parameters()->fileSystemTree;
			event->params().isLastEvent = true;
			Application::postEvent(parameters()->receiver, event.take());
		}
		else
		{
			QScopedPointer<Event> event(new Event(Event::ListFiles));
			event->params().fileSystemTree = parameters()->fileSystemTree;
			event->params().isLastEvent = true;
			event->params().updates = updatedFiles;
			Application::postEvent(parameters()->receiver, event.take());
		}
}
