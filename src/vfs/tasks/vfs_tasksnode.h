#ifndef VFS_TASKSNODE_H_
#define VFS_TASKSNODE_H_

#include "vfs_basetask.h"
#include "containers/vfs_tasksmap.h"
#include "../actions/asynchronous/vfs_asyncfileaction.h"
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
	virtual void updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed) = 0;
	virtual void completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed) = 0;
	virtual void performActionEvent(const AsyncFileAction::FilesList &files, const QString &error) = 0;

protected:
	void addTask(BaseTask *task, const NodeItem::Holder &item);
	void addTask(BaseTask *task, const Snapshot &snapshot);
	void addTask(BaseTask *task, const ICopyControl *destanation, const Snapshot &snapshot);
	void resetTask(BaseTask *task, BaseTask *oldTask);
	void resetTask(BaseTask *task, BaseTask *oldTask, const ICopyControl *destanation);
	void handleTask(BaseTask *task);
	void handleTask(BaseTask *task, const ICopyControl *destanation);
	void taskHandled(BaseTask *task);
	void taskHandled(BaseTask *task, const ICopyControl *destanation);
	void cancelTask(const NodeItem::Holder &item);
	void removeAllTaskLinks(BaseTask *task);
	void removeAllTaskLinks(BaseTask *task, const ICopyControl *destanation);
	TasksMap::List cancelTaskAndTakeItems(const NodeItem::Holder &item);

private:
	TasksMap m_tasks;
};

VFS_NS_END

#endif /* VFS_TASKSNODE_H_ */
