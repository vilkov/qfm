#include "filesystemtasksnode.h"
#include "concrete/perform/performactiontask.h"
#include "tools/filesystemtaskdialog.h"
#include "tools/filesystemtaskprogressevents.h"
#include "../../application.h"


FILE_SYSTEM_NS_BEGIN

TasksNode::TasksNode(const NodeModelContainer &conteiner, Node *parent) :
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

void TasksNode::addTask(BaseTask *task, const TasksItemList &items)
{
	m_tasks.add(task, items);
	addLink();
	Application::taskPool()->handle(task);
}

void TasksNode::resetTask(BaseTask *task, BaseTask *oldTask)
{
	m_tasks.resetTask(task, oldTask);
	Application::taskPool()->handle(task);
}

void TasksNode::handleTask(BaseTask *task)
{
	addLink();
	Application::taskPool()->handle(task);
}

void TasksNode::taskHandled()
{
	removeLink();
}

void TasksNode::cancelTask(NodeItem *item)
{
	if (BaseTask *task = m_tasks.take(item))
		task->cancel();
}

void TasksNode::removeAllTaskLinks(BaseTask *task)
{
	m_tasks.removeAll(task);
	removeLink();
}

TasksMap::List TasksNode::cancelTaskAndTakeItems(NodeItem *item)
{
	TasksMap::List res;

	if (BaseTask *task = m_tasks.take(item, res))
		task->cancel();

	return res;
}

FILE_SYSTEM_NS_END
