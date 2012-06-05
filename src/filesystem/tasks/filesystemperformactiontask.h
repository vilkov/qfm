#ifndef FILESYSTEMPERFORMACTIONTASK_H_
#define FILESYSTEMPERFORMACTIONTASK_H_

#include "filesystembasetask.h"
#include "tools/taskprogress.h"
#include "../actions/asynchronous/filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class PerformActionTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
	public:
		typedef BaseTask::Event::Type Type;

	public:
		Event(BaseTask *task, Type type, const AsyncFileAction::FilesList &files, bool canceled, const QString &error) :
			BaseTask::Event(task, static_cast<Type>(type), canceled),
			files(files),
			error(error)
		{}

		AsyncFileAction::FilesList files;
		QString error;
	};

public:
	PerformActionTask(TasksNode *receiver, const AsyncFileAction::FilesList &files);

protected:
	virtual void run(const volatile Flags &aborted);
	virtual void process(const volatile Flags &aborted, QString &error) = 0;

protected:
	const AsyncFileAction::FilesList &files() const { return m_files; }

private:
	AsyncFileAction::FilesList m_files;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPERFORMACTIONTASK_H_ */
