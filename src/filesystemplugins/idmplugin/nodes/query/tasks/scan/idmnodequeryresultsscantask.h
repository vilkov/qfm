#ifndef IDMNODEQUERYRESULTSSCANTASK_H_
#define IDMNODEQUERYRESULTSSCANTASK_H_

#include "../../../../idmplugin_ns.h"
#include "../../../../../../filesystem/tasks/scan/scanfilesbasetask.h"


IDM_PLUGIN_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	ScanFilesTask(TasksNode *receiver);

	virtual void run(const volatile bool &aborted);
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSSCANTASK_H_ */
