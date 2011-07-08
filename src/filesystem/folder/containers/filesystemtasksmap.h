#ifndef FILESYSTEMTASKSMAP_H_
#define FILESYSTEMTASKSMAP_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "../../../tools/taskspool/task.h"


class TasksMap
{
public:
	TasksMap()
	{}

	void add(TasksPool::Task *task, const QString &fileName)
	{
		m_tasks[task].push_back(fileName);
		m_names[fileName] = task;
	}
	void add(TasksPool::Task *task, const QStringList &names)
	{
		m_tasks[task] = names;

		for (QStringList::size_type i = 0, size = names.size(); i < size; ++i)
			m_names[names.at(i)] = task;
	}
	void remove(const QString &name)
	{
		if (TasksPool::Task *task = m_names.value(name, 0))
		{
			QStringList &list = m_tasks[task];
			QStringList::size_type index;

			if ((index = list.indexOf(name)) != -1)
				list.removeAt(index);

			m_names.remove(name);
		}
	}
	void removeAll(const QString &name)
	{
		if (TasksPool::Task *task = m_names.value(name, 0))
		{
			QStringList list = m_tasks.take(task);

			for (QStringList::size_type i = 0, size = list.size(); i < size; ++i)
				m_names.remove(list.at(i));
		}
	}
	TasksPool::Task *take(const QString &name)
	{
		if (TasksPool::Task *task = m_names.value(name, 0))
		{
			QStringList list = m_tasks.take(task);

			for (QStringList::size_type i = 0, size = list.size(); i < size; ++i)
				m_names.remove(list.at(i));

			return task;
		}

		return 0;
	}

private:
	typedef QMap<TasksPool::Task*, QStringList> Tasks;
	typedef QMap<QString, TasksPool::Task*>     Names;

private:
	Tasks m_tasks;
	Names m_names;
};

#endif /* FILESYSTEMTASKSMAP_H_ */
