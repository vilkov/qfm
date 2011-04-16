#include "scanfilestask.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>


ScanFilesTask::ScanFilesTask(Params *params, QObject *controller1) :
	FilesTask(params, controller1)
{
	Q_ASSERT(params->entry);
}

ScanFilesTask::ScanFilesTask(Params *params, QObject *controller1, QObject *controller2) :
	FilesTask(params, controller1, controller2)
{
	Q_ASSERT(params->entry);
}

ScanFilesTask::ScanFilesTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3) :
	FilesTask(params, controller1, controller2, controller3)
{
	Q_ASSERT(params->entry);
}

void ScanFilesTask::run(const volatile bool &stopedFlag)
{
	QScopedPointer<FileSystemTree> subtree(new FileSystemTree(parameters()->entry->fileInfo().absoluteFilePath()));
	scan(subtree.data(), stopedFlag);
	parameters()->subtree = subtree.take();
}

void ScanFilesTask::scan(FileSystemTree *tree, const volatile bool &stopedFlag)
{
	QFileInfo info;
	QDirIterator dirIt(tree->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while (!stopedFlag && !shouldTerminate() && dirIt.hasNext())
	{
		dirIt.next();
		tree->add<FileSystemEntry>(getInfo(info = dirIt.fileInfo()));

		if (!info.isSymLink())
			if (info.isDir())
			{
				QScopedPointer<FileSystemTree> subtree(new FileSystemTree(info.absoluteFilePath(), tree));
				scan(subtree.data(), stopedFlag);
				tree->setSubtree(subtree.take());
			}
			else
				parameters()->size += info.size();
	}
}


ScanFilesForSizeTask::ScanFilesForSizeTask(Params *params) :
	ScanFilesTask(params, params->receiver)
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !shouldTerminate())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForSize));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->receiver, event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(Params *params) :
	ScanFilesTask(params, params->receiver)
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !shouldTerminate())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForRemove));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->receiver, event.take());
	}
}


ScanFilesWithDestinationTask::ScanFilesWithDestinationTask(Params *params, QObject *controller1) :
	ScanFilesTask(params, controller1)
{}

ScanFilesWithDestinationTask::ScanFilesWithDestinationTask(Params *params, QObject *controller1, QObject *controller2) :
	ScanFilesTask(params, controller1, controller2)
{}

ScanFilesWithDestinationTask::ScanFilesWithDestinationTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3) :
	ScanFilesTask(params, controller1, controller2, controller3)
{}

ScanFilesForCopyTask::ScanFilesForCopyTask(Params *params) :
	ScanFilesWithDestinationTask(params, params->receiver, params->destination)
{}

void ScanFilesForCopyTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !shouldTerminate())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForCopy));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		event->params().destinationDirectory = parameters()->destinationDirectory;
		Application::postEvent(parameters()->receiver, event.take());
	}
}

ScanFilesForMoveTask::ScanFilesForMoveTask(Params *params) :
	ScanFilesWithDestinationTask(params, params->receiver, params->destination)
{}

void ScanFilesForMoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !shouldTerminate())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForMove));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		event->params().destinationDirectory = parameters()->destinationDirectory;
		Application::postEvent(parameters()->receiver, event.take());
	}
}
