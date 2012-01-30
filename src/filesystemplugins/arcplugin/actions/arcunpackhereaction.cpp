#include "arcunpackhereaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackHereAction::UnPackHereAction() :
	AsyncFileAction(Application::desktopService()->unpackActionIcon(16), tr("Unpack here"))
{}

bool UnPackHereAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackHereAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
