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
#ifndef DEFAULT_FILESBASETASK_H_
#define DEFAULT_FILESBASETASK_H_

#include <vfs/tasks/vfs_basetask.h>
#include <vfs/containers/vfs_snapshot.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class FilesBaseTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			UpdateFiles = BaseTask::Event::User,
			ScanFilesForSize = BaseTask::Event::User + 1,
			ScanFilesForRemove = BaseTask::Event::User + 2,
			ScanFilesForCopy = BaseTask::Event::User + 3,
			RemoveFiles = BaseTask::Event::User + 4,
			CopyFiles = BaseTask::Event::User + 5,
			SearchFiles_NewFile = BaseTask::Event::User + 6,
			SearchFiles_Done = BaseTask::Event::User + 7
		};

	public:
		Event(BaseTask *task, Type type, bool canceled, const Snapshot &snapshot) :
			BaseTask::Event(task, static_cast<BaseTask::Event::Type>(type), canceled),
			snapshot(snapshot)
		{}

		Snapshot snapshot;
	};

	class ExtendedEvent : public BaseTask::ExtendedEvent
	{
		Q_DISABLE_COPY(ExtendedEvent)

	public:
		typedef FilesBaseTask::Event::Type Type;

	public:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot) :
			BaseTask::ExtendedEvent(task, static_cast<BaseTask::Event::Type>(type), destination, canceled),
			snapshot(snapshot)
		{}

		Snapshot snapshot;
	};

public:
	FilesBaseTask(TasksNode *receiver);
	FilesBaseTask(TasksNode *receiver, ICopyControl::Holder &destination);
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_FILESBASETASK_H_ */
