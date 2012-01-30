#include "arcunpackintosubdiraction.h"


ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	m_action(tr("Unpack into subdir"), 0)
{}

const QAction *UnPackIntoSubdirAction::action() const
{
	return &m_action;
}

void UnPackIntoSubdirAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
