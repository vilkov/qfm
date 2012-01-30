#include "arcunpackintosubdiraction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	AsyncFileAction(Application::desktopService()->unpackActionIcon(16), tr("Unpack into subfolder"))
{}

bool UnPackIntoSubdirAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackIntoSubdirAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
