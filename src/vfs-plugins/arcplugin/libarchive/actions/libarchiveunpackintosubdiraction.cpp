#include "libarchiveunpackintosubdiraction.h"
#include "tasks/libarchiveunpackintosubdiractiontask.h"

#include <desktop/theme/desktop_theme.h>


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), ::Desktop::Theme::current()->unpackActionIcon(), tr("Unpack into subfolder"))
{}

PerformActionTask *UnPackIntoSubdirAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return new UnPackIntoSubdirActionTask(receiver, container, files);
}

LIBARCHIVE_ARC_PLUGIN_NS_END
