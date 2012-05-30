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
			IFileContainerScanner::IEnumerator::Holder enumerator;
			m_snapshot.container()->scanner()->enumerate(enumerator);

			if (enumerator)
			{
				WrappedNodeItem::Holder wrappedItem;
				DefaultNodeItem::Holder item;
				IFileInfo::Holder info;
				QTime base = QTime::currentTime();
				QTime current;

				while (enumerator->next() && !aborted)
				{
					current = QTime::currentTime();

					if (item = m_snapshot.exists(enumerator->fileName()))
						if (enumerator->isObsolete(item.as<DefaultNodeItem>()->info().data()))
							m_snapshot.insert(enumerator->fileName(), new WrappedNodeItem(m_snapshot.container(), info = enumerator->info(), NULL));
						else
							m_snapshot.insert(enumerator->fileName(), new WrappedNodeItem());
					else
						m_snapshot.insert(enumerator->fileName(), new WrappedNodeItem(m_snapshot.container(), info = enumerator->info(), NULL));

					if (base.msecsTo(current) > 300)
					{
						Snapshot localUpdates(takeUpdates(m_snapshot));
						base = current;

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

DEFAULT_PLUGIN_NS_END
