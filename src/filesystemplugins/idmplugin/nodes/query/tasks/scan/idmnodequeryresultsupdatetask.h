#ifndef IDMNODEQUERYRESULTSUPDATETASK_H_
#define IDMNODEQUERYRESULTSUPDATETASK_H_

#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../../filesystem/tasks/filesystembasetask.h"


IDM_PLUGIN_NS_BEGIN

class UpdateFilesTask : public BaseTask
{
public:
	UpdateFilesTask(TasksNode *receiver, const Snapshot &snapshot);

	virtual void run(const volatile Flags &aborted);

protected:
	Snapshot scan(const volatile Flags &aborted);

private:
	Snapshot m_snapshot;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSUPDATETASK_H_ */
