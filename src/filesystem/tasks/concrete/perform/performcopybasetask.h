#ifndef PERFORMCOPYBASETASK_H_
#define PERFORMCOPYBASETASK_H_

#include <QtCore/QCoreApplication>
#include "../filesystemfilesextendedbasetask.h"
#include "../../tools/taskprogress.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyBaseTask : public FilesExtendedBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	class Event : public FilesExtendedBaseTask::Event
	{
	public:
		Event(BaseTask *task, Type type, ICopyControl::Holder &destination, bool canceled, const Snapshot &snapshot, bool move) :
			FilesExtendedBaseTask::Event(task, type, destination, canceled, snapshot),
			move(move)
		{}

		bool move;
	};

public:
	PerformCopyBaseTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot);

protected:
	Snapshot copy(const volatile Flags &aborted);
	void copyEntry(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

	virtual void copyFile(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted) = 0;

protected:
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;
	QString m_lastError;
	TaskProgress m_progress;

private:
	Snapshot m_snapshot;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYBASETASK_H_ */
