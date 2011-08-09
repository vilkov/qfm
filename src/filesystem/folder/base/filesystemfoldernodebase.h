#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include "containers/filesystemtasksmap.h"
#include "../../filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeBase : public Node
{
public:
	FolderNodeBase(Node *parent = 0);

protected:
	const TasksMap &tasks() const { return m_tasks; }
	TasksMap &tasks() { return m_tasks; }

private:
	TasksMap m_tasks;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
