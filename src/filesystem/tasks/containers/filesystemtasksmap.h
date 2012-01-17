#ifndef FILESYSTEMTASKSMAP_H_
#define FILESYSTEMTASKSMAP_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "../filesystembasetask.h"
#include "../items/filesystemtasknodeitem.h"


FILE_SYSTEM_NS_BEGIN

class TasksMap
{
public:
	typedef QList<TaskNodeItem *> List;

public:
	TasksMap()
	{}

	void add(BaseTask *task, TaskNodeItem *item)
	{
		m_tasks[task].push_back(item);
		m_items[item] = task;
	}
	void add(BaseTask *task, const List &items)
	{
		m_tasks[task] = items;

		for (List::size_type i = 0, size = items.size(); i < size; ++i)
			m_items[items.at(i)] = task;
	}
	void remove(TaskNodeItem *item)
	{
		if (BaseTask *task = m_items.value(item, 0))
		{
			List &list = m_tasks[task];
			List::size_type index;

			if ((index = list.indexOf(item)) != -1)
				list.removeAt(index);

			m_items.remove(item);

			if (list.isEmpty())
				m_tasks.remove(task);
		}
	}
	void removeAll(BaseTask *task)
	{
		List list = m_tasks.take(task);

		for (List::size_type i = 0, size = list.size(); i < size; ++i)
			m_items.remove(list.at(i));
	}
	BaseTask *take(TaskNodeItem *item)
	{
		if (BaseTask *task = m_items.value(item, 0))
		{
			List list = m_tasks.take(task);

			for (List::size_type i = 0, size = list.size(); i < size; ++i)
				m_items.remove(list.at(i));

			return task;
		}

		return 0;
	}
	void resetTask(BaseTask *task, BaseTask *oldTask)
	{
		const List &list = (m_tasks[task] = m_tasks.take(oldTask));

		for (List::size_type i = 0, size = list.size(); i < size; ++i)
			m_items[list.at(i)] = task;
	}

private:
	typedef QMap<BaseTask *, List>           Tasks;
	typedef QMap<TaskNodeItem *, BaseTask *> Items;

private:
	Tasks m_tasks;
	Items m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSMAP_H_ */
