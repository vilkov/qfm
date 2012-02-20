#include "arcunpackintosubdiraction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), Application::desktopService()->unpackActionIcon(16), tr("Unpack into subfolder"))
{}

PerformActionTask *UnPackIntoSubdirAction::process(TasksNode *receiver, const FilesList &files) const
{
	return NULL;
}

ARC_PLUGIN_NS_END
