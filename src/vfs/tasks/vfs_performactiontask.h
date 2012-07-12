#ifndef VFS_PERFORMACTIONTASK_H_
#define VFS_PERFORMACTIONTASK_H_

#include "vfs_basetask.h"
#include "tools/vfs_taskprogress.h"
#include "../actions/asynchronous/vfs_asyncfileaction.h"


VFS_NS_BEGIN

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

VFS_NS_END

#endif /* VFS_PERFORMACTIONTASK_H_ */
