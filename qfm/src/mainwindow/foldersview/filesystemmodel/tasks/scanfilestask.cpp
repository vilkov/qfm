#include "scanfilestask.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>


ScanFilesTask::ScanFilesTask(Params *parameters) :
	FilesTask(parameters)
{
	Q_ASSERT(parameters->entry);
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

	while (!stopedFlag && !isReceiverDead() && dirIt.hasNext())
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


ScanFilesForSizeTask::ScanFilesForSizeTask(Params *parameters) :
	ScanFilesTask(parameters)
{}

void ScanFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isReceiverDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForSize));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->receiver, event.take());
	}
}


ScanFilesForRemoveTask::ScanFilesForRemoveTask(Params *parameters) :
	ScanFilesTask(parameters)
{}

void ScanFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isReceiverDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForRemove));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		Application::postEvent(parameters()->receiver, event.take());
	}
}

ScanFilesForCopyTask::ScanFilesForCopyTask(Params *parameters) :
	ScanFilesTask(parameters)
{}

void ScanFilesForCopyTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isReceiverDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForCopy));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->receiver, event.take());
	}
}

ScanFilesForMoveTask::ScanFilesForMoveTask(Params *parameters) :
	ScanFilesForCopyTask(parameters)
{}

void ScanFilesForMoveTask::run(const volatile bool &stopedFlag)
{
	ScanFilesTask::run(stopedFlag);

	if (!stopedFlag && !isReceiverDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForMove));
		event->params().fileSystemTree = parameters()->fileSystemTree;
		event->params().size = parameters()->size;
		event->params().entry = parameters()->entry;
		event->params().subtree = parameters()->subtree;
		event->params().destination = parameters()->destination;
		Application::postEvent(parameters()->receiver, event.take());
	}
}
