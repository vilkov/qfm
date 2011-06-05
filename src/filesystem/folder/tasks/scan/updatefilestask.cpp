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

	QSet<Values::size_type> affected;
	UpdatesList updatedFiles = parameters()->list;
	QDirIterator dirIt(parameters()->node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

	while(!stopedFlag && !isControllerDead() && dirIt.hasNext())
	{
		current = QTime::currentTime();

#ifndef Q_OS_WIN
		affected.insert(updatedFiles.update(m_permissions.getInfo(dirIt.next())));
#else
		affected.insert(updatedFiles.update(dirIt.next()));
#endif

		if (base.msecsTo(current) > 300)
		{
			base = current;

			if (!updatedFiles.isEmpty())
			{
				Application::postEvent(parameters()->node, new Event(false, updatedFiles));
				updatedFiles = parameters()->list;
			}
		}
	}

	if (!stopedFlag && !isControllerDead())
	{
		updatedFiles.update(affected);
		Application::postEvent(parameters()->node, new Event(true, updatedFiles));
	}
}

FILE_SYSTEM_NS_END
