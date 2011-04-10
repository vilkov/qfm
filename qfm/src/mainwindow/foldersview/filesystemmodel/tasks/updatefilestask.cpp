#include "updatefilestask.h"
#include "../items/filesystementry.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>
#include <QScopedPointer>


UpdateFilesTask::UpdateFilesTask(Params *parameters) :
	FilesTask(parameters)
{
	Q_ASSERT(!parameters->list.isEmpty());
}

void UpdateFilesTask::run(const volatile bool &stopedFlag)
{
	IconProvider &iconProvider = Application::instance()->iconProvider();
	iconProvider.lock();

	QTime base = QTime::currentTime();
	QTime current;

	QFileInfo localInfo;
	ChangesList updatedFiles;
	updatedFiles.reserve(parameters()->list.size());
	ChangesList::size_type index;
	QDirIterator dirIt(parameters()->fileSystemTree->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while(!stopedFlag && !isReceiverDead() && dirIt.hasNext())
	{
		current = QTime::currentTime();

		if ((index = indexOf(dirIt.next(), parameters()->list)) != -1)
		{
			localInfo = dirIt.fileInfo();

			if (localInfo.lastModified() != static_cast<FileSystemEntry*>(parameters()->list.at(index).entry())->fileInfo().lastModified())
				updatedFiles.push_back(Change(Change::Updated, parameters()->list.at(index).entry(), getInfo(localInfo)));

			parameters()->list.removeAt(index);
		}
		else
			updatedFiles.push_back(Change(Change::Added, getInfo(dirIt.fileInfo())));

		if (base.msecsTo(current) > 300)
		{
			if (!updatedFiles.isEmpty())
			{
				QScopedPointer<Event> event(new Event(Event::UpdateFiles));
				event->params().fileSystemTree = parameters()->fileSystemTree;
				event->params().isLastEvent = false;
				event->params().updates = updatedFiles;
				Application::postEvent(parameters()->receiver, event.take());
				updatedFiles.clear();
			}

			base = current;
		}
	}

	if (!stopedFlag && !isReceiverDead())
	{
		for (ChangesList::size_type i = 0, size = parameters()->list.size(); i < size; ++i)
			updatedFiles.push_back(Change(Change::Deleted, parameters()->list.at(i).entry()));

		if (updatedFiles.isEmpty())
		{
			QScopedPointer<Event> event(new Event(Event::UpdateFiles));
			event->params().fileSystemTree = parameters()->fileSystemTree;
			event->params().isLastEvent = true;
			Application::postEvent(parameters()->receiver, event.take());
		}
		else
		{
			QScopedPointer<Event> event(new Event(Event::UpdateFiles));
			event->params().fileSystemTree = parameters()->fileSystemTree;
			event->params().isLastEvent = true;
			event->params().updates = updatedFiles;
			Application::postEvent(parameters()->receiver, event.take());
		}
	}

	iconProvider.unlock();
}

ChangesList::size_type UpdateFilesTask::indexOf(const QString &directoryPath, const ChangesList &list) const
{
	for (ChangesList::size_type i = 0, size = list.size(); i < size; ++i)
		if (static_cast<const FileSystemEntry*>(list.at(i).entry())->fileInfo().absoluteFilePath() == directoryPath)
			return i;

	return -1;
}
