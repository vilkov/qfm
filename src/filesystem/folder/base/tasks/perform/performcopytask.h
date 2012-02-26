#ifndef PERFORMCOPYTASK_H_
#define PERFORMCOPYTASK_H_

#include <QtCore/QCoreApplication>
#include "../../events/filesystemmodelevent.h"
#include "../../../../tasks/tools/taskprogress.h"
#include "../../../../tasks/filesystembasetask.h"
#include "../../../../tasks/containers/filesystemscanedfiles.h"
#include "../../../../tasks/interfaces/filesystemicopycontrol.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyTask : public BaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	enum { FileReadWriteGranularity = 16 * 1024 * 1024 };

	class Event : public BaseTask::Event
	{
	public:
		Event(BaseTask *task, bool canceled, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control, bool move) :
			BaseTask::Event(task, static_cast<Type>(ModelEvent::CopyFiles)),
			entries(entries),
			control(control.take()),
			canceled(canceled),
			move(move)
		{}

		BaseTask *task;
		ScanedFiles entries;
		PScopedPointer<ICopyControl> control;
		bool canceled;
		bool move;
	};

public:
	PerformCopyTask(TasksNode *receiver, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control, bool move);

	virtual void run(const volatile Flags &aborted);

protected:
	void copyEntry(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	void copyFile(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

private:
	ScanedFiles m_entries;
	PScopedPointer<ICopyControl> m_control;
	bool m_move;
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;
	QString m_lastError;
	TaskProgress m_progress;

private:
	PScopedPointer<IFileAccessor> m_destFile;
	PScopedPointer<IFileAccessor> m_sourceFile;
	PScopedPointer<IFile> m_destEntry;
	IFile::size_type m_readed;
	IFile::size_type m_written;
	IFile::value_type m_buffer[FileReadWriteGranularity];
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYTASK_H_ */
