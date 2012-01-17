#ifndef FILESYSTEMTASKSNODE_H_
#define FILESYSTEMTASKSNODE_H_

#include "containers/filesystemtasksmap.h"
#include "../filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

/*
 * Implements general interaction with tasks.
 *
 */

class TasksNode : public Node
{
public:
	typedef TasksMap::List TasksItemList;

public:
	TasksNode(const ModelContainer &conteiner, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

protected:
	virtual void updateProgressEvent(const TaskNodeItem *item, quint64 progress, quint64 timeElapsed) = 0;
	virtual void completedProgressEvent(const TaskNodeItem *item, quint64 timeElapsed) = 0;

protected:
	void addTask(BaseTask *task, const TasksItemList &items);
	void resetTask(BaseTask *task, BaseTask *oldTask);
	void handleTask(BaseTask *task);
	void taskHandled();
	void cancelTask(TaskNodeItem *item);
	void removeAllTaskLinks(BaseTask *task);

private:
	TasksMap m_tasks;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSNODE_H_ */
