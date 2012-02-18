#include "arcunpackaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackAction::UnPackAction() :
	Action(tr("Unpacking..."), Application::desktopService()->unpackActionIcon(16), tr("Unpack..."))
{}

bool UnPackAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &aborted) const
{

}

ARC_PLUGIN_NS_END
