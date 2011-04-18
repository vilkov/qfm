#include "scanfilestask.h"
#include "../../events/filesystemmodelevents.h"
#include "../../../../../application.h"
#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>


ScanFilesTask::ScanFilesTask(Params *params, QObject *controller1) :
	FilesTask(params, controller1)
{
	Q_ASSERT(params->source.entry);
	Q_ASSERT(params->source.object);
	Q_ASSERT(params->source.fileSystemTree);
}

ScanFilesTask::ScanFilesTask(Params *params, QObject *controller1, QObject *controller2) :
	FilesTask(params, controller1, controller2)
{
	Q_ASSERT(params->source.entry);
	Q_ASSERT(params->source.object);
	Q_ASSERT(params->source.fileSystemTree);
}

ScanFilesTask::ScanFilesTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3) :
	FilesTask(params, controller1, controller2, controller3)
{
	Q_ASSERT(params->source.entry);
	Q_ASSERT(params->source.object);
	Q_ASSERT(params->source.fileSystemTree);
}

void ScanFilesTask::run(const volatile bool &stopedFlag)
{
	QScopedPointer<FileSystemTree> subtree(new FileSystemTree(parameters()->source.entry->fileInfo().absoluteFilePath()));
	scan(subtree.data(), stopedFlag);
	parameters()->subtree = subtree.take();
}

void ScanFilesTask::scan(FileSystemTree *tree, const volatile bool &stopedFlag)
{
	QFileInfo info;
	QDirIterator dirIt(tree->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while (!stopedFlag && !isControllerDead() && dirIt.hasNext())
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
	ScanFilesTask(params, params->source.object)
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForSize));
		event->params().snapshot.fileSystemTree = parameters()->source.fileSystemTree;
		event->params().size = parameters()->size;
		event->params().snapshot.entry = parameters()->source.entry;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->source.object, event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(Params *params) :
	ScanFilesTask(params, params->source.object)
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForRemove));
		event->params().snapshot.fileSystemTree = parameters()->source.fileSystemTree;
		event->params().size = parameters()->size;
		event->params().snapshot.entry = parameters()->source.entry;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->source.object, event.take());
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
	ScanFilesWithDestinationTask(params, params->source.object, params->destination.object)
{}

void ScanFilesForCopyTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForCopy));
		event->params().snapshot.fileSystemTree = parameters()->source.fileSystemTree;
		event->params().size = parameters()->size;
		event->params().snapshot.entry = parameters()->source.entry;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->source.object, event.take());
	}
}

ScanFilesForMoveTask::ScanFilesForMoveTask(Params *params) :
	ScanFilesWithDestinationTask(params, params->source.object, params->destination.object)
{}

void ScanFilesForMoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesWithDestinationTask::run(stopedFlag);

	if (!stopedFlag && !isControllerDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForMove));
		event->params().snapshot.fileSystemTree = parameters()->source.fileSystemTree;
		event->params().size = parameters()->size;
		event->params().snapshot.entry = parameters()->source.entry;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->source.object, event.take());
	}
}
