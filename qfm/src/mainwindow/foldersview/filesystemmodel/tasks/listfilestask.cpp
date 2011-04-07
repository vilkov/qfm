#include "listfilestask.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>


ListFilesTask::ListFilesTask(const QString &directory, QObject *receiver) :
	FilesTask(directory, receiver)
{}

void ListFilesTask::run(const volatile bool &stopedFlag)
{
    if (!directory().isEmpty())
    {
		QTime base = QTime::currentTime();
		QTime current;
		QList<FileSystemInfo> updatedFiles;
		QDirIterator dirIt(directory(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while (!stopedFlag && dirIt.hasNext())
		{
			dirIt.next();
			current = QTime::currentTime();
			updatedFiles.push_back(info(dirIt.fileInfo()));

			if (base.msecsTo(current) > 300)
			{
				Application::postEvent(receiver(), new ListFilesEvent(updatedFiles));
				updatedFiles.clear();
				base = current;
			}
		}

		if (!updatedFiles.isEmpty())
			Application::postEvent(receiver(), new ListFilesEvent(updatedFiles));
    }
}
