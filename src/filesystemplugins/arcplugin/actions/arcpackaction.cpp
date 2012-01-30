#include "arcpackaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

PackAction::PackAction() :
	AsyncFileAction(Application::desktopService()->packActionIcon(16), tr("Pack..."))
{}

bool PackAction::prepare(const FilesList &files)
{
	return true;
}

void PackAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
