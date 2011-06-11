#include "performcopytreetask.h"
#include "../../../../application.h"
#include <QtCore/QFile>


FILE_SYSTEM_NS_BEGIN

PerformCopyTreeTask::PerformCopyTreeTask(Node *receiver, ModelEvents::ScanFilesWithDestParams &params, bool move) :
	PerformCopyEntryTask(new Params(receiver, params, move))
{}

void PerformCopyTreeTask::run(const volatile bool &stopedFlag)
{
//	m_progress.init();
//
//	QDir dir(parameters()->destination.node->absoluteFilePath());
//
////	if (dir.exists() && cd(dir, parameters()->source.entry, stopedFlag))
////		copy(dir, parameters()->subnode, stopedFlag);
////	else
////		m_canceled = true;
//
//	if (!stopedFlag && !isControllerDead())
//		if (m_canceled)
//		{
//			QScopedPointer<CanceledEvent> event(new CanceledEvent());
//			event->params().snapshot = parameters()->source;
//			event->params().removeSource = parameters()->removeSource;
////			event->params().subnode = parameters()->subnode;
//			event->params().destination = parameters()->destination;
//			Application::postEvent(parameters()->source.node, event.take());
//		}
//		else
//		{
//			QScopedPointer<CompletedEvent> event(new CompletedEvent());
//			event->params().snapshot = parameters()->source;
//			event->params().removeSource = parameters()->removeSource;
////			event->params().subnode = parameters()->subnode;
//			event->params().destination = parameters()->destination;
//			Application::postEvent(parameters()->source.node, event.take());
//		}
}

bool PerformCopyTreeTask::cd(QDir &destination, FolderNodeEntry *entry, const volatile bool &stopedFlag)
{
	if (destination.cd(entry->fileName()))
		return true;
	else
	{
		bool tryAgain;

		do
			if (destination.mkdir(entry->fileName()))
				if (destination.cd(entry->fileName()))
					return true;
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								entry->lockReason(),
								tr("Failed to create directory \"%1\" in \"%2\". Skip it?").
									arg(entry->fileName()).
									arg(destination.absolutePath()),
								stopedFlag);
			else
				if (m_skipAllIfNotCopy)
					break;
				else
					askForSkipIfNotCopy(
							entry->lockReason(),
							tr("Failed to create directory \"%1\" in \"%2\". Skip it?").
								arg(entry->fileName()).
								arg(destination.absolutePath()),
							stopedFlag);
		while (m_tryAgain && !stopedFlag && !isControllerDead() && !m_canceled);
	}

	return false;
}

void PerformCopyTreeTask::copy(QDir &destination, FolderNode *tree, const volatile bool &stopedFlag)
{
	bool tryAgain;
	FolderNodeEntry *entry;

//	for (FileSystemTree::size_type i = 1, size = tree->size(); i < size && !stopedFlag && !isControllerDead() && !m_canceled; ++i)
//		if ((entry = static_cast<FileSystemEntry*>(tree->child(i)))->fileInfo().isDir())
//		{
//			if (cd(destination, entry, stopedFlag))
//			{
//				copy(destination, static_cast<FileSystemTree*>(tree->subtree(entry)), stopedFlag);
//				destination.cdUp();
//			}
//		}
//		else
//		{
//			copyFile(destination, entry, tryAgain = false, stopedFlag);
//
//			if (tryAgain)
//				--i;
//		}
}

FILE_SYSTEM_NS_END
