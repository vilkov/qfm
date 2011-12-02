#ifndef IDMQUERYRESULTSMODELEVENTS_H_
#define IDMQUERYRESULTSMODELEVENTS_H_

#include "../../../idmplugin_ns.h"
#include "../../../../../filesystem/tasks/filesystembasetask.h"


IDM_PLUGIN_NS_BEGIN

struct ModelEvent
{
	enum Type
	{
		ScanFilesForRemove = BaseTask::Event::User,
		RemoveFiles = BaseTask::Event::User + 1,
	};
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTSMODELEVENTS_H_ */
