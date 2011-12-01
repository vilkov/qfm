#ifndef FILESYSTEMBASETASK_H_
#define FILESYSTEMBASETASK_H_

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QSharedData>
#include "../filesystem_ns.h"
#include "../../tools/taskspool/task.h"
#include "../../tools/threads/pmutex.h"


FILE_SYSTEM_NS_BEGIN
class TaskNode;

/*
 * This class and it's subclasses should be created only
 * in the same thread as "receiver" because of DeleteHandler!
 *
 */

class BaseTask : public Tools::TasksPool::Task
{
public:
	class Event : public QEvent
	{
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			Question = User,

			/* First user event id. */
			User = Question + 1
		};

	protected:
		Event(Type type) :
			QEvent(static_cast<QEvent::Type>(type))
		{}
	};

public:
	BaseTask(TaskNode *receiver);
	virtual ~BaseTask();

    void cancel() { m_canceled = true; }

protected:
    TaskNode *receiver() const { return m_receiver; }
	const volatile bool isCanceled() const { return m_canceled; }
	const volatile bool &isReceiverDead() const { return m_controllerDead; }

	/* Sync post event to GUI thread. */
	qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile bool &aborted) const;

private:
	struct MutexHolder : public QSharedData
	{
		PMutex mutex;
	};
	typedef QExplicitlySharedDataPointer<MutexHolder> MutexHolderPointer;
	class DeleteHandler;

private:
    MutexHolderPointer m_mutexHolder;
    TaskNode *m_receiver;
    DeleteHandler *m_handler;
	volatile bool m_canceled;
	volatile bool m_controllerDead;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASETASK_H_ */
