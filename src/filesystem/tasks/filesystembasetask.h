#ifndef FILESYSTEMBASETASK_H_
#define FILESYSTEMBASETASK_H_

#include <QtCore/QObject>
#include <QtCore/QSharedData>
#include "../filesystem_ns.h"
#include "../../tools/taskspool/task.h"
#include "../../tools/threads/pmutex.h"


FILE_SYSTEM_NS_BEGIN

/*
 * This class and it's subclasses should be created only
 * in the same thread as "receiver" because of DeleteHandler!
 *
 */

class BaseTask : public Tools::TasksPool::Task
{
public:
	BaseTask(QObject *receiver);
	virtual ~BaseTask();

	void cancel() { m_canceled = true; }

protected:
	QObject *receiver() const { return m_receiver; }
	const volatile bool isCanceled() const { return m_canceled; }
	const volatile bool &isControllerDead() const { return m_controllerDead; }

private:
	struct MutexHolder : public QSharedData
	{
		PMutex mutex;
	};
	typedef QExplicitlySharedDataPointer<MutexHolder> MutexHolderPointer;
	class DeleteHandler;

private:
    MutexHolderPointer m_mutexHolder;
	QObject *m_receiver;
    DeleteHandler *m_handler;
	volatile bool m_canceled;
	volatile bool m_controllerDead;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASETASK_H_ */
