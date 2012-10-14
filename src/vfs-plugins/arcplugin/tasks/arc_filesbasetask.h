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
#ifndef ARC_FILESBASETASK_H_
#define ARC_FILESBASETASK_H_

#include "../arc_ns.h"
#include "../../default/tasks/default_filesbasetask.h"


ARC_PLUGIN_NS_BEGIN

class FilesBaseTask : public Default::FilesBaseTask
{
public:
	typedef Default::FilesBaseTask Base;

public:
	class Event : public Base::Event
	{
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			ScanComplete = Base::Event::ScanFilesForSize,
			CopyComplete = Base::Event::CopyFiles
		};

	public:
		Event(BaseTask *task, Type type, const QString &error, bool canceled, const Snapshot &snapshot) :
			Base::Event(task, static_cast<Base::Event::Type>(type), canceled, snapshot),
			error(error)
		{}

		QString error;
	};

	class ExtendedEvent : public Base::ExtendedEvent
	{
		Q_DISABLE_COPY(ExtendedEvent)

	public:
		typedef FilesBaseTask::Event::Type Type;

	public:
		ExtendedEvent(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot) :
			Base::ExtendedEvent(task, static_cast<Base::ExtendedEvent::Type>(type), destination, canceled, snapshot)
		{}
	};

public:
	FilesBaseTask(TasksNode *receiver);
	FilesBaseTask(TasksNode *receiver, ICopyControl::Holder &destination);
};

ARC_PLUGIN_NS_END

#endif /* ARC_FILESBASETASK_H_ */
