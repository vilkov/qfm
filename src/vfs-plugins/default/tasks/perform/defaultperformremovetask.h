#ifndef DEFAULTPERFORMREMOVETASK_H_
#define DEFAULTPERFORMREMOVETASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tasks/tools/vfs_taskprogress.h>
#include "../defaultfilesbasetask.h"


DEFAULT_PLUGIN_NS_BEGIN

class PerformRemoveTask : public FilesBaseTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveBaseTask)

public:
	PerformRemoveTask(TasksNode *receiver, const Snapshot &snapshot);

protected:
	virtual void run(const volatile Flags &aborted);

protected:
	void removeEntry(WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);
	void doRemove(WrappedNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

protected:
	Snapshot m_snapshot;
	TaskProgress m_progress;

private:
	QString m_error;
	bool m_skipAllIfNotRemove;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPERFORMREMOVETASK_H_ */
