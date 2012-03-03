#include "../updatefilestask.h"

#include <dirent.h>
#include <string.h>

#include <QtCore/QDateTime>


FILE_SYSTEM_NS_BEGIN

UpdateFilesTask::UpdateFilesTask(TasksNode *receiver, IFileContainer::Holder &container, const UpdatesList &updates) :
	ScanFilesBaseTask(receiver),
	m_updates(updates),
	m_container(container)
{}

void UpdateFilesTask::run(const volatile Flags &aborted)
{
	DIR *dir;

	if (dir = opendir(m_container->location().toUtf8()))
	{
		QTime base = QTime::currentTime();
		QTime current;

		struct dirent *entry;
		UpdatesList localUpdates;

		while ((entry = readdir(dir)) != NULL)
		{
			current = QTime::currentTime();

			if (entry->d_type == DT_DIR)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
					m_updates.update(Info(m_container->location(QString::fromUtf8(entry->d_name)), true));
			}
			else
				m_updates.update(Info(m_container->location(QString::fromUtf8(entry->d_name)), true));

			if (base.msecsTo(current) > 300)
			{
				localUpdates = m_updates.takeUpdates();
				base = current;

				if (!localUpdates.isEmpty())
					postEvent(new Event(this, false, localUpdates));
			}
		}

		closedir(dir);
	}

	postEvent(new Event(this, true, m_updates.takeUpdates(), isCanceled()));
}

FILE_SYSTEM_NS_END
