#include "arcpackaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

PackAction::PackAction() :
	Action(tr("Packing..."), Application::desktopService()->packActionIcon(16), tr("Pack..."))
{}

PerformActionTask *PackAction::process(TasksNode *receiver, const FilesList &files) const
{
	return NULL;
}

ARC_PLUGIN_NS_END
