#include "updatefilestask.h"
#include "../../../../application.h"
#include <QtCore/QSet>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QDirIterator>
#include <QtCore/QScopedPointer>


FILE_SYSTEM_NS_BEGIN

UpdateFilesTask::UpdateFilesTask() :
	parent_class(new Params())
{}

void UpdateFilesTask::run(const volatile bool &stopedFlag)
{
	QTime base = QTime::currentTime();
	QTime current;

	UpdatesList localUpdates;
	UpdatesList &updatedFiles = parameters()->updates;
	QDirIterator dirIt(parameters()->node.absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while(!stopedFlag && !isControllerDead() && dirIt.hasNext())
	{
		current = QTime::currentTime();

#ifndef Q_OS_WIN
		updatedFiles.update(m_permissions.getInfo(dirIt.next()));
#else
		updatedFiles.update(dirIt.next());
#endif

		if (base.msecsTo(current) > 300)
		{
			localUpdates = updatedFiles.takeUpdates();
			base = current;

			if (!localUpdates.isEmpty())
				Application::postEvent(parameters()->receiver, new Event(false, localUpdates));
		}
	}

	if (!stopedFlag && !isControllerDead())
		Application::postEvent(parameters()->receiver, new Event(true, updatedFiles.takeUpdates()));
}

FILE_SYSTEM_NS_END
