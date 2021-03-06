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
#include "vfs_tasksnode.h"
#include "vfs_performactiontask.h"
#include "tools/vfs_taskdialog.h"
#include "tools/vfs_taskprogressevents.h"
#include "../../application.h"


VFS_NS_BEGIN

TasksNode::TasksNode(const Container &conteiner, Node *parent) :
	Node(conteiner, parent)
{}

bool TasksNode::event(QEvent *e)
{
	switch (static_cast<BaseTask::Event::Type>(e->type()))
	{
		case BaseTask::Event::Question:
		{
			QuestionEvent *event = static_cast<QuestionEvent *>(e);

			event->accept();
			event->showDialog(Application::mainWindow());

			return true;
		}

		case BaseTask::Event::UserInput:
		{
			UserInputEvent *event = static_cast<UserInputEvent *>(e);

			event->accept();
			event->showDialog(Application::mainWindow());

			return true;
		}

		case BaseTask::Event::Progress:
		{
			typedef UpdateProgressEvent * NotConstEvent;
			typedef const UpdateProgressEvent * Event;
			Event event = static_cast<Event>(e);
			e->accept();

			updateProgressEvent(event->item, event->progress, event->timeElapsed);

			return true;
		}

		case BaseTask::Event::Completed:
		{
			typedef CompletedProgressEvent * NotConstEvent;
			typedef const CompletedProgressEvent * Event;
			Event event = static_cast<Event>(e);
			e->accept();

			completedProgressEvent(event->item, event->timeElapsed);

			return true;
		}

		case BaseTask::Event::Action:
		{
			typedef PerformActionTask::Event * NotConstEvent;
			typedef const PerformActionTask::Event * Event;
			Event event = static_cast<Event>(e);
			e->accept();

			performActionEvent(event->files, event->error);
			removeAllTaskLinks(event->task);

			return true;
		}

		default:
			return Node::event(e);
	}
}

void TasksNode::addTask(BaseTask *task, const Item::Holder &item)
{
	m_tasks.add(task, item);
	addLinks(1);
	Application::taskPool()->handle(task);
}

void TasksNode::addTask(BaseTask *task, const Snapshot &snapshot)
{
	m_tasks.add(task, snapshot);
	addLinks(1);
	Application::taskPool()->handle(task);
}

void TasksNode::addTask(BaseTask *task, const ICopyControl *destanation, const Snapshot &snapshot)
{
	m_tasks.add(task, snapshot);
	addLinks(1);
	destanation->node()->addLinks(1);
	Application::taskPool()->handle(task);
}

void TasksNode::resetTask(BaseTask *task, BaseTask *oldTask)
{
	m_tasks.resetTask(task, oldTask);
	Application::taskPool()->handle(task);
}

void TasksNode::resetTask(BaseTask *task, BaseTask *oldTask, const ICopyControl *destanation)
{
	m_tasks.resetTask(task, oldTask);
	destanation->node()->removeLink();
	Application::taskPool()->handle(task);
}

void TasksNode::handleTask(BaseTask *task)
{
	addLinks(1);
	Application::taskPool()->handle(task);
}

void TasksNode::handleTask(BaseTask *task, const ICopyControl *destanation)
{
	addLinks(1);
	destanation->node()->addLinks(1);
	Application::taskPool()->handle(task);
}

void TasksNode::taskHandled(BaseTask *task)
{
	Q_UNUSED(task);
	removeLink();
}

void TasksNode::taskHandled(BaseTask *task, const ICopyControl *destanation)
{
	destanation->node()->removeLink();
	removeLink();
}

void TasksNode::cancelTask(const Item::Holder &item)
{
	if (BaseTask *task = m_tasks.take(item))
		task->cancel();
}

void TasksNode::removeAllTaskLinks(BaseTask *task)
{
	m_tasks.removeAll(task);
	removeLink();
}

void TasksNode::removeAllTaskLinks(BaseTask *task, const ICopyControl *destanation)
{
	m_tasks.removeAll(task);
	destanation->node()->removeLink();
	removeLink();
}

TasksMap::List TasksNode::cancelTaskAndTakeItems(const Item::Holder &item)
{
	TasksMap::List res;

	if (BaseTask *task = m_tasks.take(item, res))
		task->cancel();

	return res;
}

VFS_NS_END
