#include "defaultscanfilestask.h"
#include "../../model/items/defaultnodeitem.h"

#include <QtCore/QDateTime>


DEFAULT_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(Event::Type type, TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_type(type),
	m_snapshot(snapshot),
	m_move(false)
{}

ScanFilesTask::ScanFilesTask(Event::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move) :
	FilesBaseTask(receiver, destination),
	m_type(type),
	m_snapshot(snapshot),
	m_move(move)
{}

void ScanFilesTask::run(const volatile Flags &aborted)
{
	switch (m_type)
	{
		case Event::UpdateFiles:
		{
			QString error;
			IFileContainerScanner::IEnumerator::Holder enumerator;

			if (enumerator = m_snapshot.container()->scanner()->enumerate(error))
			{
				WrappedNodeItem::Holder wrappedItem;
				Snapshot::iterator item;
				IFileInfo::Holder info;
				const IFileInfo *current;
				QTime baseTime = QTime::currentTime();
				QTime currentTime;

				for (Snapshot::iterator item, end = m_snapshot.end(); (current = enumerator->next()) && !aborted;)
				{
					currentTime = QTime::currentTime();

					if ((item = m_snapshot.find(current->fileName())) != end)
						if (isObsolete((*item).first.as<NodeItem>()->info(), current))
							m_snapshot.insert(item, new WrappedNodeItem(m_snapshot.container(), info = enumerator->info(), NULL));
						else
							m_snapshot.insert(item, new WrappedNodeItem());
					else
						m_snapshot.insert(current->fileName(), new WrappedNodeItem(m_snapshot.container(), info = enumerator->info(), NULL));

					if (baseTime.msecsTo(currentTime) > 300)
					{
						Snapshot localUpdates(takeUpdates(m_snapshot));
						baseTime = currentTime;

						if (!localUpdates.isEmpty())
							postEvent(new UpdatesEvent(this, false, localUpdates, false));
					}
				}
			}

			postEvent(new UpdatesEvent(this, true, m_snapshot, aborted));
			break;
		}

		case Event::ScanFilesForSize:
		case Event::ScanFilesForRemove:
		{
			m_snapshot.container()->scanner()->scan(m_snapshot, aborted, m_error);
			postEvent(new Event(this, static_cast<Event::Type>(m_type), aborted, m_snapshot));
			break;
		}

		case Event::ScanFilesForCopy:
		{
			m_snapshot.container()->scanner()->scan(m_snapshot, aborted, m_error);
			postEvent(new CopyEvent(this, static_cast<CopyEvent::Type>(m_type), destination(), aborted, m_snapshot, m_move));
			break;
		}

		default:
		{
			postEvent(new Event(this, static_cast<Event::Type>(m_type), aborted, Snapshot()));
			break;
		}
	}
}

Snapshot ScanFilesTask::takeUpdates(Snapshot &snapshot)
{
	Snapshot::Updates res(snapshot);

	for (Snapshot::iterator it = snapshot.begin(), end = snapshot.end(); it != end;)
		if ((*it).second)
		{
			res.add(it.key(), (*it));
			it = snapshot.remove(it);
		}
		else
			++it;

	return res;
}

bool ScanFilesTask::isObsolete(const IFileInfo *oldInfo, const IFileInfo *newInfo)
{
	return  oldInfo->lastModified() != newInfo->lastModified() ||
			oldInfo->fileSize() != newInfo->fileSize() ||
			oldInfo->permissions() != newInfo->permissions() ||
			oldInfo->fileType()->name().isEmpty();
}

DEFAULT_PLUGIN_NS_END
