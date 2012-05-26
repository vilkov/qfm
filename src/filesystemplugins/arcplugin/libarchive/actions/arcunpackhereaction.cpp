#include "arcunpackhereaction.h"
#include "../../../../application.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

UnPackHereAction::UnPackHereAction() :
	Action(tr("Unpacking..."), Application::desktopService()->unpackActionIcon(16), tr("Unpack here"))
{}

PerformActionTask *UnPackHereAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return NULL;
}

LIBARCHIVE_ARC_PLUGIN_NS_END
