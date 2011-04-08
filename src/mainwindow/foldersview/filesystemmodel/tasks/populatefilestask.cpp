#include "populatefilestask.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>


PopulateFilesTask::PopulateFilesTask(FileSystemTree *tree, const QString &directory, QObject *receiver) :
	FilesTask(tree, directory, receiver),
	m_subtree(0)
{}

void PopulateFilesTask::run(const volatile bool &stopedFlag)
{
    if (!directory().isEmpty())
    {
    	IconProvider &iconProvider = Application::instance()->iconProvider();
    	iconProvider.lock();

    	populate(m_subtree = new FileSystemTree(directory()), stopedFlag);

    	iconProvider.unlock();
		Application::postEvent(receiver(), new PopulateFilesForRemoveEvent(tree(), m_subtree));
    }
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
			tree->setSubtree(tree->last(), subtree.data());
			populate(subtree.take(), stopedFlag);
		}
	}
}
