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
#ifndef IDM_QUERYRESULTSNODE_SCANTASK_H_
#define IDM_QUERYRESULTSNODE_SCANTASK_H_

#include "../../events/idm_queryresultsmodelevents.h"
#include "../../../../../default/tasks/default_filesbasetask.h"


IDM_PLUGIN_NS_BEGIN

class ScanFilesTask : public ::VFS::Plugins::Default::FilesBaseTask
{
public:
	class UpdatesEvent : public Event
	{
	public:
		UpdatesEvent(BaseTask *task, const Snapshot &snapshot, bool canceled) :
			Event(task, static_cast<Type>(ModelEvent::UpdateFiles), canceled, snapshot)
		{}
	};

	class CopyEvent : public ExtendedEvent
	{
	public:
		CopyEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			ExtendedEvent(task, type, destination, canceled, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot);
	ScanFilesTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	ModelEvent::Type m_type;
	Snapshot m_snapshot;
	bool m_move;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTSNODE_SCANTASK_H_ */
