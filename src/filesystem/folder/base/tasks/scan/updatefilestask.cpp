#include "updatefilestask.h"
#include "../../../../../application.h"
#include <QtCore/QSet>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QDirIterator>


FILE_SYSTEM_NS_BEGIN

UpdateFilesTask::UpdateFilesTask(QObject *receiver, const Info &info, const UpdatesList &updates) :
	FolderBaseTask(receiver),
	m_info(info),
	m_updates(updates)
{}

void UpdateFilesTask::run(const volatile bool &aborted)
{
	QTime base = QTime::currentTime();
	QTime current;

	UpdatesList localUpdates;
	QDirIterator dirIt(m_info.absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while(!aborted && !isReceiverDead() && dirIt.hasNext())
	{
		current = QTime::currentTime();

#ifndef Q_OS_WIN
		m_updates.update(m_permissions.getInfo(dirIt.next()));
#else
		m_updates.update(dirIt.next());
#endif

		if (base.msecsTo(current) > 300)
		{
			localUpdates = m_updates.takeUpdates();
			base = current;

			if (!localUpdates.isEmpty())
				Application::postEvent(receiver(), new Event(false, localUpdates));
		}
	}

	if (!aborted && !isReceiverDead())
		Application::postEvent(receiver(), new Event(true, m_updates.takeUpdates()));
}

FILE_SYSTEM_NS_END
