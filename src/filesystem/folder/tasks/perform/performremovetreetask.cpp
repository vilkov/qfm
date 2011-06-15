#include "performremovetreetask.h"
#include "../../../../application.h"
#include <QtCore/QDir>


FILE_SYSTEM_NS_BEGIN

PerformRemoveTreeTask::PerformRemoveTreeTask(Node *receiver, ModelEvents::ScanFilesForRemoveEvent::Params &params) :
	PerformRemoveEntryTask(new Params(receiver, params))
{}

PerformRemoveTreeTask::PerformRemoveTreeTask(Node *receiver, ModelEvents::CopyTreeFilesCompletedEvent::Params &params) :
	PerformRemoveEntryTask(new Params(receiver, params))
{}

void PerformRemoveTreeTask::run(const volatile bool &stopedFlag)
{
	m_progress.init();

	remove(parameters()->subnode.data(), stopedFlag);

	if (!stopedFlag && !isControllerDead())
		if (m_canceled)
			postCanceledEvent();
		else
			if (m_removeParentEntry)
			{
				bool tryAgain;

				do
					removeEntry(parameters()->source.entry, tryAgain = false, stopedFlag);
				while (tryAgain && !isControllerDead() && !stopedFlag && !m_canceled);

				if (!stopedFlag && !isControllerDead())
					if (m_canceled)
						postCanceledEvent();
					else
						postCompletedEvent();
			}
			else
				postCompletedEvent();
}

void PerformRemoveTreeTask::remove(FileSystemList *node, const volatile bool &stopedFlag)
{
	bool tryAgain;

	for (FileSystemList::size_type i = 0, size = node->size(); i < size && !stopedFlag && !isControllerDead() && !m_canceled; ++i)
		if (node->at(i)->isList())
		{
			remove(static_cast<FileSystemList*>(node->at(i)), stopedFlag);

			if (!m_canceled)
			{
				removeEntry(node->at(i), tryAgain = false, stopedFlag);

				if (tryAgain)
					--i;
			}
		}
		else
		{
			removeEntry(node->at(i), tryAgain = false, stopedFlag);

			if (tryAgain)
				--i;
		}
}

FILE_SYSTEM_NS_END
