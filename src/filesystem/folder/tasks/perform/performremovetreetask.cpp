#include "performremovetreetask.h"
#include "../../../../application.h"
#include <QtCore/QDir>


FILE_SYSTEM_NS_BEGIN

PerformRemoveTreeTask::PerformRemoveTreeTask(Params *params) :
	PerformRemoveEntryTask(params)
{}

void PerformRemoveTreeTask::run(const volatile bool &stopedFlag)
{
	m_progress.init();

//	remove(parameters()->subnode, stopedFlag);

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

void PerformRemoveTreeTask::remove(FolderNode *node, const volatile bool &stopedFlag)
{
	bool tryAgain;

//	for (FileSystemTree::size_type i = 1, size = node->size(); i < size && !stopedFlag && !isControllerDead() && !m_canceled; ++i)
//		if (static_cast<FileSystemEntry*>(node->child(i))->fileInfo().isDir())
//		{
//			remove(static_cast<FileSystemTree*>(node->subnode(node->child(i))), stopedFlag);
//
//			if (!m_canceled)
//			{
//				removeEntry(static_cast<FileSystemEntry*>(node->child(i)), tryAgain = false, stopedFlag);
//
//				if (tryAgain)
//					--i;
//			}
//		}
//		else
//		{
//			removeEntry(static_cast<FileSystemEntry*>(node->child(i)), tryAgain = false, stopedFlag);
//
//			if (tryAgain)
//				--i;
//		}
}

FILE_SYSTEM_NS_END
