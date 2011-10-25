#ifndef FILESYSTEMTASKSNODE_H_
#define FILESYSTEMTASKSNODE_H_

#include <QtCore/QStringList>
#include "filesystembasetask.h"
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
	TasksNode(const ModelContainer &conteiner, Node *parent = 0);

protected:
	void addTask(BaseTask *task, const QStringList &files);
	void resetTask(BaseTask *task, const QString &fileName);
	void handleTask(BaseTask *task);
	void cancelTask(const QString &fileName);
	void removeTask(const QString &fileName);
	void removeTaskAll(const QString &fileName);

private:
	TasksMap m_tasks;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSNODE_H_ */
