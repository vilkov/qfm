#ifndef CONTROLABLETASK_H_
#define CONTROLABLETASK_H_

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QSharedData>
#include <QtCore/QVarLengthArray>
#include "../../../../tools/taskspool/task.h"


class ControlableTask : public TasksPool::Task
{
public:
	struct Params : public QSharedData, public MemoryManagerTag
	{
	private:
		friend class ControlableTask;
	    friend class DeleteHandler;
		QMutex m_mutex;
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
	inline volatile bool shouldTerminate() const
	{
		memcmp(&m_handlers, &m_verificationHandlers, sizeof(DeleteHandler *) * MaxNumberOfControllers) != 0;
	}

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
				m_task->m_handlers[m_index] = 0;
    	}

    private:
    	friend class ControlableTask;
    	size_type m_index;
    	ControlableTask *m_task;
    	ParamsPointer m_params;
    };

private:
    ParamsPointer m_params;
    DeleteHandler *m_handlers[MaxNumberOfControllers];
    DeleteHandler *m_verificationHandlers[MaxNumberOfControllers];
};

#endif /* CONTROLABLETASK_H_ */
