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
#ifndef VFS_TASKSNODEITEM_H_
#define VFS_TASKSNODEITEM_H_

#include "../../../model/items/vfs_item.h"


VFS_NS_BEGIN

class TasksNodeItem : public Item
{
public:
	TasksNodeItem(Base *parent);

	bool isInProgress() const { return !m_done.isNull(); }
	bool isCompleted() const { return m_done == m_total; }

	quint64 done() const { return m_done.toULongLong(); }
	quint64 total() const { return m_total; }
	quint64 timeElapsed() const { return m_timeElapsed; }

	qint32 progress() const { return (m_done.toULongLong() * 100) / m_total; }

protected:
	void start(quint64 total) { m_done = quint64(1); m_total = total < 1 ? 1 : total; }
	void progress(quint64 value, quint64 timeElapsed) { m_done = value; m_timeElapsed = timeElapsed; }
	void stop() { m_done.clear(); m_total = 0; m_timeElapsed = 0; }

private:
	QVariant m_done;
	quint64 m_total;
	quint64 m_timeElapsed;
};

VFS_NS_END

#endif /* VFS_TASKSNODEITEM_H_ */
