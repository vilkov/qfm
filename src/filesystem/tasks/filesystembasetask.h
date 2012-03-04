#ifndef FILESYSTEMBASETASK_H_
#define FILESYSTEMBASETASK_H_

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QSharedData>
#include "../filesystem_ns.h"
#include "../../tools/taskspool/task.h"
#include "../../tools/threads/pmutex.h"


FILE_SYSTEM_NS_BEGIN
class TasksNode;


/*
 * This class and subclasses of this class should be created only
 * in the same thread as "receiver" because of DeleteHandler!
 *
 */

class BaseTask : public ::Tools::TasksPool::Task
{
public:
	class Event : public QEvent
	{
		Q_DISABLE_COPY(Event)

	public:
		enum Type
		{
			Question = User,
			Progress = User + 1,
			Completed = User + 2,
			Action = User + 3,

			/* First user event id. */
			User = Action + 1
		};

	public:
		BaseTask *task;
		bool canceled;

	protected:
		Event(BaseTask *task, Type type) :
			QEvent(static_cast<QEvent::Type>(type)),
			task(task),
			canceled(false)
		{}
		Event(BaseTask *task, Type type, bool canceled) :
			QEvent(static_cast<QEvent::Type>(type)),
			task(task),
			canceled(canceled)
		{}
	};

public:
	BaseTask(TasksNode *receiver);
	virtual ~BaseTask();

    void cancel() { m_canceled = true; }

protected:
	/* Posts event to receiver(). */
	void postEvent(Event *event) const;

	/* Sync post event to the GUI thread. */
	qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile Flags &aborted) const;

private:
	struct MutexHolder : public QSharedData
	{
		PMutex mutex;
	};
	typedef QExplicitlySharedDataPointer<MutexHolder> MutexHolderPointer;
	class DeleteHandler;

private:
    MutexHolderPointer m_mutexHolder;
    TasksNode *m_receiver;
    DeleteHandler *m_handler;
	Flag m_canceled;
	Bit m_canceledBit;
	Flag m_controllerDead;
	Bit m_controllerDeadBit;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASETASK_H_ */
