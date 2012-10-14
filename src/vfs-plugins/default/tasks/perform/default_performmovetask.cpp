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
#include "default_performmovetask.h"


DEFAULT_PLUGIN_NS_BEGIN

PerformMoveTask::PerformMoveTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot) :
	PerformCopyTask(receiver, destination, snapshot, true)
{}

void PerformMoveTask::copyFile(const IFileContainer *destination, SnapshotItem *entry, volatile bool &tryAgain, const volatile Flags &aborted)
{
	if (destination->move(entry->container(), entry->info(), m_lastError))
		m_progress.update(entry->info()->fileSize());
	else
		PerformCopyTask::copyFile(destination, entry, tryAgain = false, aborted);
}

DEFAULT_PLUGIN_NS_END
