#include "defaultscanfilestask.h"
#include "../../model/items/defaultnodeitem.h"

#include <QtCore/QDateTime>


DEFAULT_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_type(type),
	m_snapshot(snapshot)
{}

void ScanFilesTask::run(const volatile Flags &aborted)
{
	switch (m_type)
	{
		case ModelEvent::UpdateFiles:
		{
			IFileContainerScanner::IEnumerator::Holder enumerator;
			m_snapshot.container()->scanner()->enumerate(enumerator);

			if (enumerator)
			{
				WrappedNodeItem::Holder wrappedItem;
				DefaultNodeItem *item;
				QTime base = QTime::currentTime();
				QTime current;

				while (enumerator->next() && !aborted)
				{
					current = QTime::currentTime();

					if (item = static_cast<DefaultNodeItem *>(m_snapshot.exists(enumerator->fileName())))
						if (enumerator->isObsolete(item->info().data()))
							m_snapshot.insert(enumerator->fileName(), new WrappedNodeItem(m_snapshot.container(), enumerator->create()));
						else
							m_snapshot.insert(enumerator->fileName(), new WrappedNodeItem());
					else
						m_snapshot.insert(enumerator->fileName(), new WrappedNodeItem(m_snapshot.container(), enumerator->create()));

					if (base.msecsTo(current) > 300)
					{
						Snapshot::Updates localUpdates = m_snapshot.takeUpdates();
						base = current;

						if (!localUpdates.isEmpty())
							postEvent(new UpdatesEvent(this, false, localUpdates));
					}
				}
			}

			postEvent(new UpdatesEvent(this, true, m_snapshot.takeUpdates()));
			break;
		}

		case ModelEvent::ScanFilesForSize:
		case ModelEvent::ScanFilesForRemove:
		case ModelEvent::ScanFilesForCopy:
		{
			break;
		}

		default:
		{
			postEvent(new Event(this, static_cast<Event::Type>(m_type), aborted, Snapshot()));
			break;
		}
	}
}


ScanFilesExtendedTask::ScanFilesExtendedTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move) :
	FilesExtendedBaseTask(receiver, destination),
	m_type(type),
	m_snapshot(snapshot),
	m_move(move)
{}

void ScanFilesExtendedTask::run(const volatile Flags &aborted)
{

}

DEFAULT_PLUGIN_NS_END
