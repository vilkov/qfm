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
#ifndef VFS_TASKSNODEDELEGATE_H_
#define VFS_TASKSNODEDELEGATE_H_

#include "items/vfs_tasksnodeitem.h"
#include "../../model/vfs_delegate.h"


VFS_NS_BEGIN

class TasksNodeDelegate : public Delegate
{
	Q_DISABLE_COPY(TasksNodeDelegate)

public:
	TasksNodeDelegate(QObject *parent = 0);

protected:
    void paintProgressInMb(const TasksNodeItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const;
};

VFS_NS_END

#endif /* VFS_TASKSNODEDELEGATE_H_ */
