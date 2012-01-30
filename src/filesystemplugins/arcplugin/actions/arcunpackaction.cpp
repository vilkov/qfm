#include "arcunpackaction.h"


ARC_PLUGIN_NS_BEGIN

UnPackAction::UnPackAction() :
	m_action(tr("Unpack..."), 0)
{}

const QAction *UnPackAction::action() const
{
	return &m_action;
}

bool UnPackAction::prepare(const FilesList &files)
{
	return true;
}

void UnPackAction::process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const
{

}

ARC_PLUGIN_NS_END
