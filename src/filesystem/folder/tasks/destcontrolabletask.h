#ifndef DESTCONTROLABLETASK_H_
#define DESTCONTROLABLETASK_H_

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include "basetask.h"


FILE_SYSTEM_NS_BEGIN

class DestControlableTask : public BaseTask
{
public:
	/*
	 * This class and it's subclasses should be created only
	 * in the same thread as "destination.object" because of DeleteHandler!
	 *
	 */
	DestControlableTask(QObject *receiver);
	virtual ~DestControlableTask();

protected:
    inline QObject *receiver() const { return m_receiver; }
	inline const volatile bool &isControllerDead() const { return m_controllerDead; }

private:
	struct MutexHolder : public QSharedData
	{
		QMutex mutex;
	};
	typedef QExplicitlySharedDataPointer<MutexHolder> MutexHolderPointer;

	class DeleteHandler : public QObject
    {
    public:
    	DeleteHandler(DestControlableTask *task, QObject *parent) :
    		QObject(parent),
    		mutexHolder(task->m_mutexHolder),
    		task(task)
    	{}
    	virtual ~DeleteHandler()
    	{
    		QMutexLocker locker(&mutexHolder->mutex);

    		if (task != 0)
    		{
				task->m_handler = 0;
				task->m_receiver = 0;
				task->m_controllerDead = true;
    		}
    	}

    	/*
    	 * Just holds the reference to DestControlableTask::m_mutexHolder,
    	 * so it can not be deleted before this object.
    	 */
    	MutexHolderPointer mutexHolder;
    	DestControlableTask *task;
    };

private:
    MutexHolderPointer m_mutexHolder;
    QObject *m_receiver;
    DeleteHandler *m_handler;
	volatile bool m_controllerDead;
};

FILE_SYSTEM_NS_END

#endif /* DESTCONTROLABLETASK_H_ */
