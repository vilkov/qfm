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
#ifndef VFS_ASYNCFILEACTION_H_
#define VFS_ASYNCFILEACTION_H_

#include "../vfs_fileaction.h"
#include "../../tasks/vfs_basetask.h"


VFS_NS_BEGIN
class TasksNode;
class PerformActionTask;


/**
 * This class and subclasses must be reentrant!
 */
class AsyncFileAction : public FileAction
{
public:
	typedef BaseTask::Flags Flags;

public:
	AsyncFileAction(const QIcon &icon, const QString &text);

	virtual bool isAsynchronous() const;

	virtual const QString &lockReason() const = 0;
	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const = 0;
};

VFS_NS_END

#endif /* VFS_ASYNCFILEACTION_H_ */
