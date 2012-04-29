#ifndef DEFAULTPERFORMCOPYTASK_H_
#define DEFAULTPERFORMCOPYTASK_H_

#include <QtCore/QCoreApplication>
#include "../defaultfilesbasetask.h"
#include "../../../../filesystem/tasks/tools/taskprogress.h"


DEFAULT_PLUGIN_NS_BEGIN

class PerformCopyTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

public:
	class Event : public FilesBaseTask::ExtendedEvent
	{
	public:
		Event(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			FilesBaseTask::ExtendedEvent(task, type, destination, canceled, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	PerformCopyTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot, bool move);

protected:
	virtual void run(const volatile Flags &aborted);

	void copyEntry(const IFileContainer *destination, WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void copyFile(const IFileContainer *destination, WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	QString m_lastError;
	TaskProgress m_progress;

private:
	Snapshot m_snapshot;
	bool m_move;

	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;

	PScopedPointer<IFileAccessor> m_destFile;
	PScopedPointer<IFileAccessor> m_sourceFile;
	IFileAccessor::size_type m_readed;
	IFileAccessor::size_type m_written;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPERFORMCOPYTASK_H_ */
