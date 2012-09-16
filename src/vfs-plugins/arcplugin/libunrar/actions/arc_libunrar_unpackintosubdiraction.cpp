#include "arc_libunrar_unpackintosubdiraction.h"
#include "tasks/arc_libunrar_unpackintosubdiractiontask.h"

#include <desktop/theme/desktop_theme.h>


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), ::Desktop::Theme::current()->unpackActionIcon(), tr("Unpack into subfolder"))
{}

PerformActionTask *UnPackIntoSubdirAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return new UnPackIntoSubdirActionTask(receiver, container, files);
}

ARC_PLUGIN_LIBUNRAR_NS_END
