#include "updatefilestask.h"
#include "../../../../application.h"
#include <QtCore/QSet>
#include <QDir>
#include <QDateTime>
#include <QDirIterator>
#include <QScopedPointer>


FILE_SYSTEM_NS_BEGIN

UpdateFilesTask::UpdateFilesTask(ParamsPointer &params) :
	parent_class(params.take())
{
	Q_ASSERT(params->node);
}

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

		affected.insert(updatedFiles.update(dirIt.fileInfo(), m_userId, m_groupId));

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
