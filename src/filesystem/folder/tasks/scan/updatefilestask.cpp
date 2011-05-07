#include "updatefilestask.h"
#include "../../../../application.h"
#include <QDir>
#include <QDateTime>
#include <QDirIterator>
#include <QScopedPointer>


FILE_SYSTEM_NS_BEGIN

UpdateFilesTask::UpdateFilesTask(Params *params) :
	parent_class(params)
{
	Q_ASSERT(params->node);
}

void UpdateFilesTask::run(const volatile bool &stopedFlag)
{
	QTime base = QTime::currentTime();
	QTime current;

	QFileInfo localInfo;
	ChangesList updatedFiles;
	updatedFiles.reserve(parameters()->list.size());
	ChangesList::size_type index;
	QDirIterator dirIt(parameters()->node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while(!stopedFlag && !isControllerDead() && dirIt.hasNext())
	{
		current = QTime::currentTime();

		if ((index = indexOf(dirIt.next(), parameters()->list)) != -1)
		{
			localInfo = dirIt.fileInfo();

			if (localInfo.lastModified() != parameters()->list.at(index).entry()->lastModified())
				updatedFiles.push_back(Change(Change::Updated, parameters()->list.at(index).entry(), getInfo(localInfo)));

			parameters()->list.removeAt(index);
		}
		else
			updatedFiles.push_back(Change(Change::Added, getInfo(dirIt.fileInfo())));

		if (base.msecsTo(current) > 300)
		{
			if (!updatedFiles.isEmpty())
			{
				QScopedPointer<Event> event(new Event());
				event->params().isLastEvent = false;
				event->params().updates = updatedFiles;
				Application::postEvent(parameters()->node, event.take());
				updatedFiles.clear();
			}

			base = current;
		}
	}

	if (!stopedFlag && !isControllerDead())
	{
		for (ChangesList::size_type i = 0, size = parameters()->list.size(); i < size; ++i)
			updatedFiles.push_back(Change(Change::Deleted, parameters()->list.at(i).entry()));

		if (updatedFiles.isEmpty())
		{
			QScopedPointer<Event> event(new Event());
			event->params().isLastEvent = true;
			Application::postEvent(parameters()->node, event.take());
		}
		else
		{
			QScopedPointer<Event> event(new Event());
			event->params().isLastEvent = true;
			event->params().updates = updatedFiles;
			Application::postEvent(parameters()->node, event.take());
		}
	}
}

ChangesList::size_type UpdateFilesTask::indexOf(const QString &directoryPath, const ChangesList &list) const
{
	for (ChangesList::size_type i = 0, size = list.size(); i < size; ++i)
		if (list.at(i).entry()->absoluteFilePath() == directoryPath)
			return i;

	return -1;
}

FILE_SYSTEM_NS_END
