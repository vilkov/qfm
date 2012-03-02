#ifndef PERFORMCOPYBASETASK_H_
#define PERFORMCOPYBASETASK_H_

#include <QtCore/QCoreApplication>
#include "../filesystembasetask.h"
#include "../tools/taskprogress.h"
#include "../containers/filesystemscanedfiles.h"
#include "../interfaces/filesystemicopycontrol.h"
#include "../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyBaseTask : public BaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	class Event : public BaseTask::Event
	{
	public:
		Event(BaseTask *task, Type type, bool canceled, const ScanedFiles &files, PScopedPointer<ICopyControl> &control, bool move) :
			BaseTask::Event(task, type),
			control(control.take()),
			files(files),
			move(move)
		{
			this->canceled = canceled;
		}

		PScopedPointer<ICopyControl> control;
		ScanedFiles files;
		bool move;
	};

public:
	PerformCopyBaseTask(TasksNode *receiver, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control);

	virtual void run(const volatile Flags &aborted);

protected:
	void copyEntry(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void copyFile(IFileContainer *destination, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted) = 0;

protected:
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	PScopedPointer<ICopyControl> &control() { return m_control; }
	ScanedFiles &files() { return m_files; }

protected:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;
	QString m_lastError;
	TaskProgress m_progress;

private:
	PScopedPointer<ICopyControl> m_control;
	ScanedFiles m_files;

private:
	PScopedPointer<IFileAccessor> m_destFile;
	PScopedPointer<IFileAccessor> m_sourceFile;
	PScopedPointer<IFile> m_destEntry;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYBASETASK_H_ */
