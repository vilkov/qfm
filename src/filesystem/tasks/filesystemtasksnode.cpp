#include "filesystemtasksnode.h"
#include "filesystemperformactiontask.h"
#include "tools/filesystemtaskdialog.h"
#include "tools/filesystemtaskprogressevents.h"
#include "../../application.h"


FILE_SYSTEM_NS_BEGIN

TasksNode::TasksNode(const Container &conteiner, Node *parent) :
	Node(conteiner, parent)
{}

bool TasksNode::event(QEvent *e)
{
	switch (static_cast<BaseTask::Event::Type>(e->type()))
	{
		case BaseTask::Event::Question:
		{
			QuestionEvent *event = static_cast<QuestionEvent*>(e);

			event->accept();
			event->result()->lock();
			event->result()->setAnswer(QMessageBox::question(Application::mainWindow(), event->title(), event->question(), QMessageBox::StandardButtons(event->buttons())));
			event->result()->unlock();

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

			performActionEvent(event->files);
			removeAllTaskLinks(event->task);

			return true;
		}

		default:
			break;
	}

	return Node::event(e);
}

void TasksNode::addTask(BaseTask *task, const NodeItem::Holder &item)
{
	m_tasks.add(task, item);
	addLink();
	Application::taskPool()->handle(task);
}

void TasksNode::addTask(BaseTask *task, const Snapshot &snapshot)
{
	m_tasks.add(task, snapshot);
	addLink();
	Application::taskPool()->handle(task);
}

void TasksNode::addTask(BaseTask *task, const ICopyControl *destanation, const Snapshot &snapshot)
{
	m_tasks.add(task, snapshot);
	addLink();
	destanation->node()->addLink();
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
	addLink();
	Application::taskPool()->handle(task);
}

void TasksNode::handleTask(BaseTask *task, const ICopyControl *destanation)
{
	addLink();
	destanation->node()->addLink();
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

void TasksNode::cancelTask(const NodeItem::Holder &item)
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

TasksMap::List TasksNode::cancelTaskAndTakeItems(const NodeItem::Holder &item)
{
	TasksMap::List res;

	if (BaseTask *task = m_tasks.take(item, res))
		task->cancel();

	return res;
}

FILE_SYSTEM_NS_END
