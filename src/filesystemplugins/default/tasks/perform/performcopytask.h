#ifndef PERFORMCOPYTASK_H_
#define PERFORMCOPYTASK_H_

#include "../../events/filesystemmodelevent.h"
#include "../../../../tasks/concrete/perform/performcopybasetask.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyTask : public PerformCopyBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

	class Event : public PerformCopyBaseTask::Event
	{
	public:
		Event(BaseTask *task, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			PerformCopyBaseTask::Event(task, static_cast<Type>(ModelEvent::CopyFiles), destination, canceled, snapshot, move)
		{}
	};

public:
	PerformCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

	virtual void run(const volatile Flags &aborted);

protected:
	virtual void copyFile(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

private:
	bool m_move;

private:
	PScopedPointer<IFileAccessor> m_destFile;
	PScopedPointer<IFileAccessor> m_sourceFile;
	IFileAccessor::size_type m_readed;
	IFileAccessor::size_type m_written;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYTASK_H_ */
