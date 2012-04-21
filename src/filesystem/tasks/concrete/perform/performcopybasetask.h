#ifndef PERFORMCOPYBASETASK_H_
#define PERFORMCOPYBASETASK_H_

#include <QtCore/QCoreApplication>
#include "../filesystemfilesbasetask.h"
#include "../../tools/taskprogress.h"


FILE_SYSTEM_NS_BEGIN

class PerformCopyBaseTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformCopyTask)

public:
	class Event : public FilesBaseTask::Event
	{
	public:
		Event(BaseTask *task, Type type, bool canceled, const Snapshot &snapshot, IFileContainer::Holder &destination, bool move) :
			FilesBaseTask::Event(task, type, canceled, snapshot),
			destination(destination.take()),
			move(move)
		{}

		IFileContainer::Holder destination;
		bool move;
	};

public:
	PerformCopyBaseTask(TasksNode *receiver, const Snapshot &snapshot, IFileContainer::Holder &destination);

protected:
	Snapshot copy(const volatile Flags &aborted);
	void copyEntry(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

	virtual void copyFile(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted) = 0;

protected:
	void askForOverwrite(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	void askForSkipIfNotCopy(const QString &title, const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	IFileContainer::Holder &destination() { return m_destination; }

protected:
	bool m_skipAllIfNotCreate;
	bool m_skipAllIfNotCopy;
	bool m_doNotOverwriteAll;
	bool m_overwriteAll;
	QString m_lastError;
	TaskProgress m_progress;

private:
	Snapshot m_snapshot;
	IFileContainer::Holder m_destination;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMCOPYBASETASK_H_ */
