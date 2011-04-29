#include "performremovetreetask.h"
#include "../../../../../application.h"
#include <QtCore/QDir>


PerformRemoveTreeTask::PerformRemoveTreeTask(Params *params) :
	PerformRemoveEntryTask(params)
{}

void PerformRemoveTreeTask::run(const volatile bool &stopedFlag)
{
	m_progress.init();

	remove(parameters()->subtree, stopedFlag);

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

void PerformRemoveTreeTask::remove(FileSystemTree *tree, const volatile bool &stopedFlag)
{
	bool tryAgain;

	for (FileSystemTree::size_type i = 1, size = tree->size(); i < size && !stopedFlag && !isControllerDead() && !m_canceled; ++i)
		if (static_cast<FileSystemEntry*>(tree->child(i))->fileInfo().isDir())
		{
			remove(static_cast<FileSystemTree*>(tree->subtree(tree->child(i))), stopedFlag);

			if (!m_canceled)
			{
				removeEntry(static_cast<FileSystemEntry*>(tree->child(i)), tryAgain = false, stopedFlag);

				if (tryAgain)
					--i;
			}
		}
		else
		{
			removeEntry(static_cast<FileSystemEntry*>(tree->child(i)), tryAgain = false, stopedFlag);

			if (tryAgain)
				--i;
		}
}
