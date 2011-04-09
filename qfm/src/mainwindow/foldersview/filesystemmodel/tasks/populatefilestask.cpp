#include "populatefilestask.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>


PopulateFilesTask::PopulateFilesTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver) :
	FilesTask(tree, receiver),
	m_size(0),
	m_entry(entry),
	m_subtree(0)
{}

void PopulateFilesTask::run(const volatile bool &stopedFlag)
{
	IconProvider &iconProvider = Application::instance()->iconProvider();
	iconProvider.lock();

	QScopedPointer<FileSystemTree> subtree(new FileSystemTree(m_entry->fileInfo().absoluteFilePath()));
	populate(subtree.data(), stopedFlag);

	iconProvider.unlock();
	m_subtree = subtree.take();
}

void PopulateFilesTask::populate(FileSystemTree *tree, const volatile bool &stopedFlag)
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
				populate(subtree.data(), stopedFlag);
				tree->setSubtree(subtree.take());
			}
			else
				m_size += info.size();
	}
}


PopulateFilesForSizeTask::PopulateFilesForSizeTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver) :
	PopulateFilesTask(tree, entry, receiver)
{}

void PopulateFilesForSizeTask::run(const volatile bool &stopedFlag)
{
	PopulateFilesTask::run(stopedFlag);

	if (!stopedFlag && !isReceiverDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForSize));
		event->params().fileSystemTree = tree();
		event->params().size = size();
		event->params().entry = entry();
		event->params().subtree = subtree();
		Application::postEvent(receiver(), event.take());
	}
}


PopulateFilesForRemoveTask::PopulateFilesForRemoveTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver) :
	PopulateFilesTask(tree, entry, receiver)
{}

void PopulateFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	PopulateFilesTask::run(stopedFlag);

	if (!stopedFlag && !isReceiverDead())
	{
		QScopedPointer<Event> event(new Event(Event::ScanFilesForRemove));
		event->params().fileSystemTree = tree();
		event->params().size = size();
		event->params().entry = entry();
		event->params().subtree = subtree();
		Application::postEvent(receiver(), event.take());
	}
}
