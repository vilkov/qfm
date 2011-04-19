#ifndef CONTROLABLETASK_H_
#define CONTROLABLETASK_H_

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QSharedData>
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../tools/taskspool/task.h"


class ControlableTask : public TasksPool::Task
{
public:
	struct Params : public QSharedData, public MemoryManagerTag
	{
		struct Listener
		{
			Listener() :
				object(0),
				fileSystemTree(0)
			{}
			Listener(QObject *object, FileSystemTree *fileSystemTree) :
				object(object),
				fileSystemTree(fileSystemTree)
			{}

			QObject *object;
			FileSystemTree *fileSystemTree;
		};
		struct Snapshot
		{
			Snapshot() :
				object(0),
				fileSystemTree(0),
				entry(0)
			{}
			Snapshot(QObject *object, FileSystemTree *fileSystemTree, FileSystemEntry *entry) :
				object(object),
				fileSystemTree(fileSystemTree),
				entry(entry)
			{}

			QObject *object;
			FileSystemTree *fileSystemTree;
			FileSystemEntry *entry;
		};

	private:
		friend class ControlableTask;
	    friend class DeleteHandler;
		QMutex m_mutex;
	};
	struct EventParams : public FileSystemModelEvent::EventParams
	{
		struct Snapshot
		{
			Snapshot() :
				fileSystemTree(0),
				entry(0)
			{}
			Snapshot(FileSystemTree *fileSystemTree, FileSystemEntry *entry) :
				fileSystemTree(fileSystemTree),
				entry(entry)
			{}
			Snapshot(const Params::Snapshot &listener) :
				fileSystemTree(listener.fileSystemTree),
				entry(listener.entry)
			{}

			Snapshot &operator=(const Params::Snapshot &other)
			{
				fileSystemTree = other.fileSystemTree;
				entry = other.entry;
				return *this;
			}

			FileSystemTree *fileSystemTree;
			FileSystemEntry *entry;
		};
	};

public:
	/*
	 * This class and it's subclasses should be created only
	 * in the same thread as "receiver" because of DeleteHandler!
	 *
	 */
	ControlableTask(Params *parameters, QObject *controller1);
	ControlableTask(Params *parameters, QObject *controller1, QObject *controller2);
	ControlableTask(Params *parameters, QObject *controller1, QObject *controller2, QObject *controller3);
	virtual ~ControlableTask();

protected:
	enum { MaxNumberOfControllers = 3 };

	/* Should be checked as well as "stopedFlag" in "run" function! */
	inline const volatile bool &isControllerDead() const { return m_isControllerDead; }

	inline Params *parameters() const { return m_params.data(); }

private:
	typedef quint8 size_type;
	typedef QExplicitlySharedDataPointer<Params> ParamsPointer;

    class DeleteHandler : public QObject
    {
    public:
    	DeleteHandler(size_type index, ControlableTask *task, QObject *parent) :
    		QObject(parent),
        	m_index(index),
    		m_task(task),
    		m_params(m_task->m_params)
    	{}
    	virtual ~DeleteHandler()
    	{
    		QMutexLocker locker(&m_params->m_mutex);

    		if (m_task != 0)
    		{
    			m_task->m_isControllerDead = true;
				m_task->m_handlers[m_index] = 0;
    		}
    	}

    private:
    	friend class ControlableTask;
    	size_type m_index;
    	ControlableTask *m_task;
    	ParamsPointer m_params;
    };

private:
    ParamsPointer m_params;
    volatile bool m_isControllerDead;
    DeleteHandler *m_handlers[MaxNumberOfControllers];
};

#endif /* CONTROLABLETASK_H_ */
