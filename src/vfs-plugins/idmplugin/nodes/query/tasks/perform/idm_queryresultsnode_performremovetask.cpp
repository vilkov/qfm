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
#include "idm_queryresultsnode_performremovetask.h"


IDM_PLUGIN_NS_BEGIN

PerformRemoveTask::PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot) :
	::VFS::Plugins::Default::PerformRemoveTask(receiver, snapshot)
{}

void PerformRemoveTask::run(const volatile Flags &aborted)
{
	bool tryAgain;
	SnapshotItem *entry;

	for (Snapshot::iterator it = m_snapshot.begin(), end = m_snapshot.end(); it != end && !aborted; ++it)
		if (entry = (*it).second)
			if (entry->info()->isDir())
			{
				m_progress.init((*it).first);
				removeEntry(entry, tryAgain = false, aborted);
				m_progress.complete();
			}
			else
			{
				m_progress.clear();
				removeEntry(entry, tryAgain = false, aborted);
			}

	postEvent(new Event(this, static_cast<Event::Type>(ModelEvent::RemoveFiles), aborted, m_snapshot));
}

IDM_PLUGIN_NS_END
