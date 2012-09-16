#include "arc_action.h"


ARC_PLUGIN_NS_BEGIN

Action::Action(const QString &lockReason, const QIcon &icon, const QString &text) :
	AsyncFileAction(icon, text),
	m_lockReason(lockReason)
{}

const QString &Action::lockReason() const
{
	return m_lockReason;
}

ARC_PLUGIN_NS_END
