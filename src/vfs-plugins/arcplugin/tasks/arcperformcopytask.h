#ifndef ARCPERFORMCOPYTASK_H_
#define ARCPERFORMCOPYTASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tasks/tools/vfs_taskprogress.h>
#include <vfs/interfaces/vfs_icopycontrol.h>
#include "arcfilesbasetask.h"


ARC_PLUGIN_NS_BEGIN

class PerformCopyTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
//	class Event : public FilesBaseTask::Event
//	{
//	public:
//		Event(BaseTask *task, bool canceled, bool move) :
//			FilesBaseTask::Event(task, CopyComplete, canceled),
//			move(move)
//		{}
//
//		ArcNodeItem::Holder item;
//		bool move;
//	};

public:
	PerformCopyTask(const QString &fileName, const Item::Holder &item, ICopyControl::Holder &control, bool move, TasksNode *receiver);

	virtual IFileAccessor::value_type *buffer() const;
	virtual IFileAccessor::size_type bufferSize() const;

	virtual void progressInit(const Item::Holder &item);
	virtual void progressUpdate(quint64 progressIncrement);
	virtual void progresscomplete();

	virtual bool overwriteAll() const;
	virtual bool skipAllIfNotCopy() const;
	virtual void askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

private:
	QString m_fileName;
	Item::Holder m_item;
	ICopyControl::Holder m_control;
	bool m_move;
	bool m_overwriteAll;
	bool m_skipAllIfNotCopy;
	TaskProgress m_progress;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

ARC_PLUGIN_NS_END

#endif /* ARCPERFORMCOPYTASK_H_ */
