#include "arcunpackhereaction.h"


ARC_PLUGIN_NS_BEGIN

UnPackHereAction::UnPackHereAction() :
	m_action(tr("Unpack here"), 0)
{}

const QAction *UnPackHereAction::action() const
{
	return &m_action;
}

void UnPackHereAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
