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
#ifndef VFS_PERFORMACTIONTASK_H_
#define VFS_PERFORMACTIONTASK_H_

#include "vfs_basetask.h"
#include "tools/vfs_taskprogress.h"
#include "../actions/asynchronous/vfs_asyncfileaction.h"


VFS_NS_BEGIN

class PerformActionTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
	public:
		typedef BaseTask::Event::Type Type;

	public:
		Event(BaseTask *task, Type type, const AsyncFileAction::FilesList &files, bool canceled, const QString &error) :
			BaseTask::Event(task, static_cast<Type>(type), canceled),
			files(files),
			error(error)
		{}

		AsyncFileAction::FilesList files;
		QString error;
	};

public:
	PerformActionTask(TasksNode *receiver, const AsyncFileAction::FilesList &files);

protected:
	virtual void run(const volatile Flags &aborted);
	virtual void process(const volatile Flags &aborted, QString &error) = 0;

protected:
	const AsyncFileAction::FilesList &files() const { return m_files; }

private:
	AsyncFileAction::FilesList m_files;
};

VFS_NS_END

#endif /* VFS_PERFORMACTIONTASK_H_ */
