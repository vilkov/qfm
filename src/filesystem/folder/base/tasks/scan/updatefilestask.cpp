#include "updatefilestask.h"
#include <QtCore/QSet>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QDirIterator>


FILE_SYSTEM_NS_BEGIN

UpdateFilesTask::UpdateFilesTask(TasksNode *receiver, const Info &info, const UpdatesList &updates) :
	ScanFilesBaseTask(receiver),
	m_info(info),
	m_updates(updates)
{}

void UpdateFilesTask::run(const volatile Flags &aborted)
{
	QTime base = QTime::currentTime();
	QTime current;

	UpdatesList localUpdates;
	QDirIterator dirIt(m_info.absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while(!aborted && dirIt.hasNext())
	{
		current = QTime::currentTime();

		m_updates.update(Info(dirIt.next(), true));

		if (base.msecsTo(current) > 300)
		{
			localUpdates = m_updates.takeUpdates();
			base = current;

			if (!localUpdates.isEmpty())
				postEvent(new Event(this, false, localUpdates));
		}
	}

	postEvent(new Event(this, true, m_updates.takeUpdates(), aborted));
}

FILE_SYSTEM_NS_END
