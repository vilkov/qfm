#ifndef PERFORMACTIONTASK_H_
#define PERFORMACTIONTASK_H_

#include "../../filesystembasetask.h"
#include "../../tools/taskprogress.h"
#include "../../../actions/asynchronous/filesystemasyncfileaction.h"


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
			BaseTask::Event(task, static_cast<Type>(type), canceled),
			files(files)
		{}

		AsyncFileAction::FilesList files;
	};

public:
	PerformActionTask(TasksNode *receiver, const AsyncFileAction::FilesList &files);

	virtual void run(const volatile Flags &aborted);

protected:
	virtual void process(const volatile Flags &aborted) = 0;

protected:
	const AsyncFileAction::FilesList &files() const { return m_files; }

private:
	AsyncFileAction::FilesList m_files;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMACTIONTASK_H_ */
