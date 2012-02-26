#include "arcunpackintosubdiraction.h"
#include "tasks/arcunpackintosubdiractiontask.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), Application::desktopService()->unpackActionIcon(16), tr("Unpack into subfolder"))
{}

PerformActionTask *UnPackIntoSubdirAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return new UnPackIntoSubdirActionTask(receiver, container, files);
}

ARC_PLUGIN_NS_END
