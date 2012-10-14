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
#ifndef DEFAULT_SCANFILESTASK_H_
#define DEFAULT_SCANFILESTASK_H_

#include "../default_filesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class ScanFilesTask : public FilesBaseTask
{
public:
	class UpdatesEvent : public Event
	{
	public:
		UpdatesEvent(BaseTask *task, bool isLastEvent, const Snapshot &snapshot, bool canceled) :
			Event(task, static_cast<Type>(Event::UpdateFiles), canceled, snapshot),
			isLastEvent(isLastEvent)
		{}

		bool isLastEvent;
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
	ScanFilesTask(Event::Type type, TasksNode *receiver, const Snapshot &snapshot);
	ScanFilesTask(Event::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

protected:
	Snapshot takeUpdates(Snapshot &snapshot);
	Snapshot takeAllUpdates(Snapshot &snapshot);
	inline bool isObsolete(const IFileInfo *oldInfo, const IFileInfo *newInfo);

private:
	Event::Type m_type;
	Snapshot m_snapshot;
	QString m_error;
	bool m_move;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_SCANFILESTASK_H_ */
