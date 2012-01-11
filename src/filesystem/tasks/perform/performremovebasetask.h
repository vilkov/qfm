#ifndef PERFORMREMOVEBASETASK_H_
#define PERFORMREMOVEBASETASK_H_

#include <QtCore/QCoreApplication>
#include "../filesystembasetask.h"
#include "../tools/taskprogress.h"
#include "../containers/filesystemscanedfiles.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveBaseTask : public BaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveBaseTask)

public:
	class Event : public BaseTask::Event
	{
	public:
		typedef BaseTask::Event::Type Type;

	public:
		Event(Type type, BaseTask *task, const ScanedFiles &files, bool canceled) :
			BaseTask::Event(static_cast<BaseTask::Event::Type>(type)),
			task(task),
			files(files),
			canceled(false)
		{}

		BaseTask *task;
		ScanedFiles files;
		bool canceled;
	};

public:
	PerformRemoveBaseTask(TasksNode *receiver, Event::Type type, const ScanedFiles &files);

	virtual void run(const volatile Flags &aborted);

protected:
	void remove(const ScanedFiles &entries, const volatile Flags &aborted);

private:
	void removeEntry(InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	void removeDir(InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	void removeFile(InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	bool doRemoveFile(const QString &filePath, QString &error);

private:
	Event::Type m_type;
	ScanedFiles m_files;
	QString m_error;
	bool m_skipAllIfNotRemove;
	TaskProgress m_progress;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVEBASETASK_H_ */
