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
#ifndef VFS_ASYNCACTION_H_
#define VFS_ASYNCACTION_H_

#include "../vfs_action.h"
#include "../../tasks/vfs_basetask.h"


VFS_NS_BEGIN

class TasksNode;
class PerformActionTask;


/**
 * This class and subclasses must be reentrant!
 */
class AsyncAction : public Action
{
public:
	typedef BaseTask::Flags Flags;

public:
	AsyncAction(const QIcon &icon, const QString &text);

	virtual bool isAsynchronous() const;

	virtual const QString &lockReason() const = 0;
	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const = 0;
};

VFS_NS_END

#endif /* VFS_ASYNCACTION_H_ */
