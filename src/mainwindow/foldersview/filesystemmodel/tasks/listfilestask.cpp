#include "listfilestask.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>


ListFilesTask::ListFilesTask(FileSystemTree *tree, QObject *receiver) :
	FilesTask(tree, receiver)
{}

void ListFilesTask::run(const volatile bool &stopedFlag)
{
	IconProvider &iconProvider = Application::instance()->iconProvider();
	iconProvider.lock();

	QTime base = QTime::currentTime();
	QTime current;
	QList<FileSystemInfo> updatedFiles;
	QDirIterator dirIt(tree()->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while (!stopedFlag && dirIt.hasNext())
	{
		dirIt.next();
		current = QTime::currentTime();
		updatedFiles.push_back(getInfo(dirIt.fileInfo()));

		if (base.msecsTo(current) > 300)
		{
			Application::postEvent(receiver(), new ListFilesEvent(tree(), updatedFiles));
			updatedFiles.clear();
			base = current;
		}
	}

	if (!updatedFiles.isEmpty())
		Application::postEvent(receiver(), new ListFilesEvent(tree(), updatedFiles));

	iconProvider.unlock();
}
