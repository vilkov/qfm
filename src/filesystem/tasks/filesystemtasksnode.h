#ifndef FILESYSTEMTASKSNODE_H_
#define FILESYSTEMTASKSNODE_H_

#include "filesystemextendedbasetask.h"
#include "containers/filesystemtasksmap.h"
#include "../actions/asynchronous/filesystemasyncfileaction.h"
#include "../filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

/*
 * Implements general interaction with tasks.
 *
 */

class TasksNode : public Node
{
	Q_DISABLE_COPY(TasksNode)

public:
	typedef TasksMap::List TasksItemList;

public:
	TasksNode(const Container &conteiner, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

protected:
	virtual void updateProgressEvent(const NodeItem *item, quint64 progress, quint64 timeElapsed) = 0;
	virtual void completedProgressEvent(const NodeItem *item, quint64 timeElapsed) = 0;
	virtual void performActionEvent(const AsyncFileAction::FilesList &files) = 0;

protected:
	void addTask(BaseTask *task, const TasksItemList &items);
	void addTask(ExtendedBaseTask *task, const TasksItemList &items);
	void resetTask(BaseTask *task, BaseTask *oldTask);
	void handleTask(BaseTask *task);
	void handleTask(ExtendedBaseTask *task);
	void taskHandled();
	void cancelTask(NodeItem *item);
	void removeAllTaskLinks(BaseTask *task);
	TasksMap::List cancelTaskAndTakeItems(NodeItem *item);

private:
	TasksMap m_tasks;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSNODE_H_ */
