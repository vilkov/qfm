#include "arcpackaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

PackAction::PackAction() :
	m_action(Application::desktopService()->packActionIcon(16), tr("Pack..."), 0)
{}

const QAction *PackAction::action() const
{
	return &m_action;
}

bool PackAction::prepare(const FilesList &files)
{
	return true;
}

void PackAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
