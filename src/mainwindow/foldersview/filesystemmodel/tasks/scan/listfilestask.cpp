#include "listfilestask.h"
#include "../../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>
#include <QScopedPointer>


ListFilesTask::ListFilesTask(Params *params) :
	parent_class(params)
{
	Q_ASSERT(params->object);
	Q_ASSERT(params->fileSystemTree);
}

void ListFilesTask::run(const volatile bool &stopedFlag)
{
	QTime base = QTime::currentTime();
	QTime current;
	QList<FileSystemInfo> updatedFiles;
	QDirIterator dirIt(parameters()->fileSystemTree->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while (!stopedFlag && !isControllerDead() && dirIt.hasNext())
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
			Application::postEvent(parameters()->object, event.take());

			updatedFiles.clear();
			base = current;
		}
	}

	if (!stopedFlag && !isControllerDead())
		if (updatedFiles.isEmpty())
		{
			QScopedPointer<Event> event(new Event(Event::ListFiles));
			event->params().fileSystemTree = parameters()->fileSystemTree;
			event->params().isLastEvent = true;
			Application::postEvent(parameters()->object, event.take());
		}
		else
		{
			QScopedPointer<Event> event(new Event(Event::ListFiles));
			event->params().fileSystemTree = parameters()->fileSystemTree;
			event->params().isLastEvent = true;
			event->params().updates = updatedFiles;
			Application::postEvent(parameters()->object, event.take());
		}
}
