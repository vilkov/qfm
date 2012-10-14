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
#include "idm_queryresultsnode_scantask.h"


IDM_PLUGIN_NS_BEGIN

ScanFilesTask::ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot) :
	FilesBaseTask(receiver),
	m_type(type),
	m_snapshot(snapshot),
	m_move(false)
{}

ScanFilesTask::ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move) :
	FilesBaseTask(receiver, destination),
	m_type(type),
	m_snapshot(snapshot),
	m_move(move)
{}

void ScanFilesTask::run(const volatile Flags &aborted)
{
	QString error;

	switch (m_type)
	{
		case ModelEvent::UpdateFiles:
		{
			IFileContainerScanner::ScanArguments args = {m_snapshot, aborted};
			m_snapshot.container()->scanner()->scan(args, error);
			postEvent(new UpdatesEvent(this, m_snapshot, aborted));
			break;
		}

		default:
		{
			IFileContainerScanner::ScanArguments args = {m_snapshot, aborted};
			m_snapshot.container()->scanner()->scan(args, error);
			postEvent(new Event(this, static_cast<Event::Type>(m_type), aborted, m_snapshot));
			break;
		}
	}
}

IDM_PLUGIN_NS_END
