#include "populatefilestask.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>


PopulateFilesTask::PopulateFilesTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver) :
	FilesTask(tree, receiver),
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

	while (!stopedFlag && dirIt.hasNext())
	{
		dirIt.next();
		tree->add<FileSystemEntry>(getInfo(info = dirIt.fileInfo()));

		if (info.isDir() && !info.isSymLink())
		{
			QScopedPointer<FileSystemTree> subtree(new FileSystemTree(info.absoluteFilePath(), tree));
			populate(subtree.data(), stopedFlag);
			tree->setSubtree(subtree.take());
		}
	}
}


PopulateFilesForRemoveTask::PopulateFilesForRemoveTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver) :
	PopulateFilesTask(tree, entry, receiver)
{}

void PopulateFilesForRemoveTask::run(const volatile bool &stopedFlag)
{
	PopulateFilesTask::run(stopedFlag);
	Application::postEvent(receiver(), new PopulateFilesForRemoveEvent(tree(), entry(), subtree()));
}
