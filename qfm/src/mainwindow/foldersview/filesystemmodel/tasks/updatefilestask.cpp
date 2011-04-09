#include "updatefilestask.h"
#include "../items/filesystementry.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>
#include <QScopedPointer>


UpdateFilesTask::UpdateFilesTask(FileSystemTree *tree, const ChangesList &list, QObject *receiver) :
	FilesTask(tree, receiver),
	m_list(list)
{}

void UpdateFilesTask::run(const volatile bool &stopedFlag)
{
	IconProvider &iconProvider = Application::instance()->iconProvider();
	iconProvider.lock();

	QTime base = QTime::currentTime();
	QTime current;

	QFileInfo localInfo;
	ChangesList updatedFiles;
	updatedFiles.reserve(m_list.size());
	ChangesList::size_type index;
	QDirIterator dirIt(tree()->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while(!stopedFlag && !isReceiverDead() && dirIt.hasNext())
	{
		current = QTime::currentTime();

		if ((index = indexOf(dirIt.next(), m_list)) != -1)
		{
			localInfo = dirIt.fileInfo();

			if (localInfo.lastModified() != static_cast<FileSystemEntry*>(m_list.at(index).entry())->fileInfo().lastModified())
				updatedFiles.push_back(Change(Change::Updated, m_list.at(index).entry(), getInfo(localInfo)));

			m_list.removeAt(index);
		}
		else
			updatedFiles.push_back(Change(Change::Added, getInfo(dirIt.fileInfo())));

		if (base.msecsTo(current) > 300)
		{
			if (!updatedFiles.isEmpty())
			{
				QScopedPointer<Event> event(new Event(Event::UpdateFiles));
				event->params().fileSystemTree = tree();
				event->params().isLastEvent = false;
				event->params().updates = updatedFiles;
				Application::postEvent(receiver(), event.take());
				updatedFiles.clear();
			}

			base = current;
		}
	}

	if (!stopedFlag && !isReceiverDead())
	{
		for (ChangesList::size_type i = 0, size = m_list.size(); i < size; ++i)
			updatedFiles.push_back(Change(Change::Deleted, m_list.at(i).entry()));

		if (updatedFiles.isEmpty())
		{
			QScopedPointer<Event> event(new Event(Event::UpdateFiles));
			event->params().fileSystemTree = tree();
			event->params().isLastEvent = true;
			Application::postEvent(receiver(), event.take());
		}
		else
		{
			QScopedPointer<Event> event(new Event(Event::UpdateFiles));
			event->params().fileSystemTree = tree();
			event->params().isLastEvent = true;
			event->params().updates = updatedFiles;
			Application::postEvent(receiver(), event.take());
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
