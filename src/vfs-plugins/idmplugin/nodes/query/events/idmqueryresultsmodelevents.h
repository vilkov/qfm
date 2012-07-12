#ifndef IDMQUERYRESULTSMODELEVENTS_H_
#define IDMQUERYRESULTSMODELEVENTS_H_

#include <vfs/tasks/vfs_basetask.h>
#include "../../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

struct ModelEvent
{
	enum Type
	{
		ScanFilesForRemove = BaseTask::Event::User,
		RemoveFiles = BaseTask::Event::User + 1,
		UpdateFiles = BaseTask::Event::User + 2
	};
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTSMODELEVENTS_H_ */
