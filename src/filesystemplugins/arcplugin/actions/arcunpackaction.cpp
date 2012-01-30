#include "arcunpackaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackAction::UnPackAction() :
	AsyncFileAction(Application::desktopService()->unpackActionIcon(16), tr("Unpack..."))
{}

bool UnPackAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
