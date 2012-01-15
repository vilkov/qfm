#ifndef ARCPERFORMCOPYTASK_H_
#define ARCPERFORMCOPYTASK_H_

#include "arctaskevent.h"
#include "../archive/arcarchive.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include "../../../filesystem/tasks/interfaces/filesystemicopycontrol.h"


ARC_PLUGIN_NS_BEGIN

class PerformCopyTask : public BaseTask, public Archive::Callback
{
public:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

	class Event : public TaskEvent
	{
	public:
		Event(BaseTask *task, bool move) :
			TaskEvent(task, CopyComplete),
			move(move)
		{}

		const ArcNodeItem::Base *item;
		bool move;
	};

public:
	PerformCopyTask(const QString &fileName, const ArcNodeItem::Base *item, PScopedPointer<ICopyControl> &control, bool move, TasksNode *receiver);

	/* Archive::Callback */
	virtual IFile::value_type *buffer() const;
	virtual IFile::size_type bufferSize() const;

	virtual bool overwriteAll() const;
	virtual bool skipAllIfNotCopy() const;
	virtual void askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	QString m_fileName;
	const ArcNodeItem::Base *m_item;
	PScopedPointer<ICopyControl> m_control;
	bool m_move;
	IFile::value_type m_buffer[FileReadWriteGranularity];
	bool m_overwriteAll;
	bool m_skipAllIfNotCopy;
};

ARC_PLUGIN_NS_END

#endif /* ARCPERFORMCOPYTASK_H_ */
