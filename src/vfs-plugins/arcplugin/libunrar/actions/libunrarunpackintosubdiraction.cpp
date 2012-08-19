#include "libunrarunpackintosubdiraction.h"
#include "tasks/libunrarunpackintosubdiractiontask.h"

#include <desktop/theme/desktop_theme.h>


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), ::Desktop::Theme::current()->unpackActionIcon(), tr("Unpack into subfolder"))
{}

PerformActionTask *UnPackIntoSubdirAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return new UnPackIntoSubdirActionTask(receiver, container, files);
}

LIBUNRAR_ARC_PLUGIN_NS_END
