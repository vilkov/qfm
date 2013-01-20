/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "default_scanfilestask.h"
#include "../../model/items/default_nodeitem.h"

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
				SnapshotItem::Holder wrappedItem;
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
							m_snapshot.insert(item, new SnapshotItem(m_snapshot.container(), info = enumerator->info(), NULL));
						else
							m_snapshot.insert(item, new SnapshotItem());
					else
						m_snapshot.insert(current->fileName(), new SnapshotItem(m_snapshot.container(), info = enumerator->info(), NULL));

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
			IFileContainerScanner::ScanArguments args = {m_snapshot, aborted};
			m_snapshot.container()->scanner()->scan(args, m_error);
			postEvent(new Event(this, static_cast<Event::Type>(m_type), aborted, m_snapshot));
			break;
		}

		case Event::ScanFilesForCopy:
		{
			IFileContainerScanner::ScanArguments args = {m_snapshot, aborted};
			m_snapshot.container()->scanner()->scan(args, m_error);
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

    for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
        if ((*i).second)
        {
            res.add(i.key(), (*i));
            i = snapshot.remove(i);
        }
        else
            ++i;

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
