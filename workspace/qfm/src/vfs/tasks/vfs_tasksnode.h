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
#ifndef VFS_TASKSNODE_H_
#define VFS_TASKSNODE_H_

#include "vfs_basetask.h"
#include "containers/vfs_tasksmap.h"
#include "../actions/asynchronous/vfs_asyncaction.h"
#include "../vfs_node.h"


VFS_NS_BEGIN

/*
 * Implements general interaction with tasks.
 *
 */

class TasksNode : public Node
{
	Q_DISABLE_COPY(TasksNode)

public:
	TasksNode(const Container &conteiner, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

protected:
	virtual void updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed) = 0;
	virtual void completedProgressEvent(const Item::Holder &item, quint64 timeElapsed) = 0;
	virtual void performActionEvent(const AsyncAction::FilesList &files, const QString &error) = 0;

protected:
	void addTask(BaseTask *task, const Item::Holder &item);
	void addTask(BaseTask *task, const Snapshot &snapshot);
	void addTask(BaseTask *task, const ICopyControl *destanation, const Snapshot &snapshot);
	void resetTask(BaseTask *task, BaseTask *oldTask);
	void resetTask(BaseTask *task, BaseTask *oldTask, const ICopyControl *destanation);
	void handleTask(BaseTask *task);
	void handleTask(BaseTask *task, const ICopyControl *destanation);
	void taskHandled(BaseTask *task);
	void taskHandled(BaseTask *task, const ICopyControl *destanation);
	void cancelTask(const Item::Holder &item);
	void removeAllTaskLinks(BaseTask *task);
	void removeAllTaskLinks(BaseTask *task, const ICopyControl *destanation);
	TasksMap::List cancelTaskAndTakeItems(const Item::Holder &item);

private:
	TasksMap m_tasks;
};

VFS_NS_END

#endif /* VFS_TASKSNODE_H_ */
