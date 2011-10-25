#include "filesystemtasksnode.h"
#include "../../application.h"


FILE_SYSTEM_NS_BEGIN

TasksNode::TasksNode(const ModelContainer &conteiner, Node *parent) :
	Node(conteiner, parent)
{}

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
