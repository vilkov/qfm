#include "arcunpackhereaction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackHereAction::UnPackHereAction() :
	m_action(Application::desktopService()->unpackActionIcon(16), tr("Unpack here"), 0)
{}

const QAction *UnPackHereAction::action() const
{
	return &m_action;
}

bool UnPackHereAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackHereAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
