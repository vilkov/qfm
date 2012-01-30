#include "arcunpackintosubdiraction.h"
#include "../../../application.h"


ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	m_action(Application::desktopService()->unpackActionIcon(16), tr("Unpack into subfolder"), 0)
{}

const QAction *UnPackIntoSubdirAction::action() const
{
	return &m_action;
}

bool UnPackIntoSubdirAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackIntoSubdirAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
