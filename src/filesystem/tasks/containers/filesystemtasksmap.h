#ifndef FILESYSTEMTASKSMAP_H_
#define FILESYSTEMTASKSMAP_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "../filesystembasetask.h"
#include "../../containers/filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

class TasksMap
{
public:
	TasksMap()
	{}

	void add(BaseTask *task, const Snapshot &snapshot)
	{
		const Snapshot::List &items = (m_tasks[task] = snapshot.list());

		for (Snapshot::List::size_type i = 0, size = items.size(); i < size; ++i)
			m_items[items.at(i).first] = task;
	}

	void remove(NodeItem *item)
	{
		if (BaseTask *task = m_items.value(item, 0))
		{
			Snapshot::List &list = m_tasks[task];
			Snapshot::List::size_type index;

			if ((index = list.indexOf(item)) != -1)
				list.removeAt(index);

			m_items.remove(item);

			if (list.isEmpty())
				m_tasks.remove(task);
		}
	}

	void removeAll(BaseTask *task)
	{
		Snapshot::List list = m_tasks.take(task);

		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
			m_items.remove(list.at(i).first);
	}

	BaseTask *take(NodeItem *item)
	{
		if (BaseTask *task = m_items.value(item, 0))
		{
			Snapshot::List list = m_tasks.take(task);

			for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
				m_items.remove(list.at(i).first);

			return task;
		}

		return NULL;
	}

	BaseTask *take(NodeItem *item, Snapshot::List &list)
	{
		if (BaseTask *task = m_items.value(item, 0))
		{
			list = m_tasks.take(task);

			for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
				m_items.remove(list.at(i).first);

			return task;
		}

		return NULL;
	}

	void resetTask(BaseTask *task, BaseTask *oldTask)
	{
		const Snapshot::List &list = (m_tasks[task] = m_tasks.take(oldTask));

		for (Snapshot::List::size_type i = 0, size = list.size(); i < size; ++i)
			m_items[list.at(i).first] = task;
	}

private:
	typedef QMap<BaseTask *, Snapshot::List> Tasks;
	typedef QMap<NodeItem *, BaseTask *>     Items;

private:
	Tasks m_tasks;
	Items m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKSMAP_H_ */
