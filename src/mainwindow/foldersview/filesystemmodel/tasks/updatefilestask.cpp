#include "updatefilestask.h"
#include "../items/filesystementry.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>


UpdateFilesTask::UpdateFilesTask(const QString &directory, const ChangesList &list, QObject *receiver) :
	FilesTask(directory, receiver),
	m_list(list)
{}

void UpdateFilesTask::run(const volatile bool &stopedFlag)
{
    if (!directory().isEmpty())
    {
		QTime base = QTime::currentTime();
	    QTime current;

	    QFileInfo localInfo;
	    ChangesList updatedFiles;
	    updatedFiles.reserve(m_list.size());
	    ChangesList::size_type index;
		QDirIterator dirIt(directory(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while(!stopedFlag && dirIt.hasNext())
		{
			current = QTime::currentTime();

			if ((index = indexOf(dirIt.next(), m_list)) != -1)
			{
				localInfo = dirIt.fileInfo();

				if (localInfo.lastModified() != static_cast<FileSystemEntry*>(m_list.at(index).entry())->fileInfo().lastModified())
					updatedFiles.push_back(Change(Change::Updated, m_list.at(index).entry(), info(localInfo)));

				m_list.removeAt(index);
			}
			else
				updatedFiles.push_back(Change(Change::Added, info(dirIt.fileInfo())));

			if (base.msecsTo(current) > 300)
			{
				if (!updatedFiles.isEmpty())
				{
					Application::postEvent(receiver(), new ChangesListEvent(updatedFiles));
					updatedFiles.clear();
				}

				base = current;
			}
		}

		if (!stopedFlag)
			for (ChangesList::size_type i = 0, size = m_list.size(); i < size; ++i)
				updatedFiles.push_back(Change(Change::Deleted, m_list.at(i).entry()));

		if (!updatedFiles.isEmpty())
			Application::postEvent(receiver(), new ChangesListEvent(updatedFiles));
    }
}

ChangesList::size_type UpdateFilesTask::indexOf(const QString &directoryPath, const ChangesList &list) const
{
	for (ChangesList::size_type i = 0, size = list.size(); i < size; ++i)
		if (static_cast<const FileSystemEntry*>(list.at(i).entry())->fileInfo().absoluteFilePath() == directoryPath)
			return i;

	return -1;
}
