#ifndef PERFORMREMOVEBASETASK_H_
#define PERFORMREMOVEBASETASK_H_

#include <QtCore/QCoreApplication>
#include "../filesystemfilesbasetask.h"
#include "../../tools/taskprogress.h"
#include "../../interfaces/filesystemicopycontrol.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveBaseTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveBaseTask)

public:
	PerformRemoveBaseTask(TasksNode *receiver, const Snapshot &snapshot);

protected:
	Snapshot remove(const volatile Flags &aborted);

private:
	void removeEntry(const IFileContainer *root, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	void doRemove(const IFileContainer *root, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

private:
	Snapshot m_snapshot;
	QString m_error;
	bool m_skipAllIfNotRemove;
	TaskProgress m_progress;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVEBASETASK_H_ */
