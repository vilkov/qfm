#ifndef PERFORMACTIONTASK_H_
#define PERFORMACTIONTASK_H_

#include "../filesystembasetask.h"
#include "../tools/taskprogress.h"
#include "../../actions/asynchronous/filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class PerformActionTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
	public:
		typedef BaseTask::Event::Type Type;

	public:
		Event(BaseTask *task, Type type, const AsyncFileAction::FilesList &files, bool canceled) :
			BaseTask::Event(task, static_cast<Type>(type)),
			files(files)
		{
			this->canceled = canceled;
		}

		AsyncFileAction::FilesList files;
	};

public:
	PerformActionTask(TasksNode *receiver, const AsyncFileAction *action, const AsyncFileAction::FilesList &files);

	virtual void run(const volatile Flags &aborted);

public:
	class Context
	{
	public:
		Context(PerformActionTask *task) :
			m_task(task)
		{}

		qint32 askUser(const QString &title, const QString &question, qint32 buttons, const volatile Flags &aborted) const { return m_task->askUser(title, question, buttons, aborted); }

		const TaskProgress &progress() const { return m_task->m_progress; }
		TaskProgress &progress() { return m_task->m_progress; }


	private:
		PerformActionTask *m_task;
	};
	friend class Context;

private:
	TaskProgress m_progress;
	const AsyncFileAction *m_action;
	AsyncFileAction::FilesList m_files;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMACTIONTASK_H_ */
