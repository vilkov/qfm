#include "filesystemtasksnode.h"
#include "tools/filesystemtaskdialog.h"
#include "tools/filesystemtaskprogressevents.h"
#include "../../application.h"


FILE_SYSTEM_NS_BEGIN

TasksNode::TasksNode(const ModelContainer &conteiner, Node *parent) :
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
			event->result()->setAnswer(QMessageBox::question(&Application::instance()->mainWindow(), event->title(), event->question(), event->buttons()));
			event->result()->unlock();

			return true;
		}
		case BaseTask::Event::Progress:
		{
			typedef UpdateProgressEvent * NotConstEvent;
			typedef const UpdateProgressEvent * Event;
			Event event = static_cast<Event>(e);
			e->accept();

			updateProgressEvent(event->fileName, event->progress, event->timeElapsed);

			return true;
		}
		case BaseTask::Event::Completed:
		{
			typedef CompletedProgressEvent * NotConstEvent;
			typedef const CompletedProgressEvent * Event;
			Event event = static_cast<Event>(e);
			e->accept();

			completedProgressEvent(event->fileName, event->timeElapsed);

			return true;
		}
		default:
			break;
	}

	return Node::event(e);
}

void TasksNode::addTask(BaseTask *task, const QStringList &files)
{
	m_tasks.add(task, files);
	addLink();
	Application::instance()->taskPool().handle(task);
}

void TasksNode::resetTask(BaseTask *task, const QString &fileName)
{
	m_tasks.resetTask(task, fileName);
	Application::instance()->taskPool().handle(task);
}

void TasksNode::handleTask(BaseTask *task)
{
	addLink();
	Application::instance()->taskPool().handle(task);
}

void TasksNode::taskHandled()
{
	removeLink();
}

void TasksNode::cancelTask(const QString &fileName)
{
	if (BaseTask *task = m_tasks.take(fileName))
		task->cancel();
}

void TasksNode::removeAllTaskLinks(const QString &fileName)
{
	m_tasks.removeAll(fileName);
	removeLink();
}

FILE_SYSTEM_NS_END
