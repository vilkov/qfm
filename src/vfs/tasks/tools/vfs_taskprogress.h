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
#ifndef VFS_TASKPROGRESS_H_
#define VFS_TASKPROGRESS_H_

#include <QtCore/QDateTime>
#include "../../model/items/vfs_item.h"


VFS_NS_BEGIN
class TasksNode;


class TaskProgress
{
public:
	TaskProgress(TasksNode *receiver);

	void init(const Item::Holder &item);
	void update(quint64 progressIncrement);
	void complete();
	void clear();

private:
	void postEvent();

private:
	Item::Holder m_item;
	quint64 m_progress;
	TasksNode *m_receiver;
	QDateTime m_baseTime;
	QDateTime m_currentTime;
	QDateTime m_startTime;
};

VFS_NS_END

#endif /* VFS_TASKPROGRESS_H_ */
