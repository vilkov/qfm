#ifndef FILESYSTEMBASETASK_H_
#define FILESYSTEMBASETASK_H_

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include "../filesystem_ns.h"
#include "../../tools/taskspool/task.h"


FILE_SYSTEM_NS_BEGIN
class TasksNode;


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

    void cancel() { m_canceled = true; }

protected:
	/* Posts event to receiver(). */
	void postEvent(Event *event) const;

	/* Sync post event to the GUI thread. */
	qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile Flags &aborted) const;

private:
    TasksNode *m_receiver;
	StaticFlag m_canceled;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASETASK_H_ */
