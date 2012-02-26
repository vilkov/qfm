#include "arcunpackaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackAction::UnPackAction() :
	Action(tr("Unpacking..."), Application::desktopService()->unpackActionIcon(16), tr("Unpack..."))
{}

PerformActionTask *UnPackAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return NULL;
}

ARC_PLUGIN_NS_END
