#include "defaultpasteaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

PasteAction::PasteAction() :
	AsyncFileAction(Application::desktopService()->pasteActionIcon(), tr("Paste")),
	m_lockReason(tr("Copying..."))
{}

const QString &PasteAction::lockReason() const
{
	return m_lockReason;
}

PerformActionTask *PasteAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return NULL;
}

DEFAULT_PLUGIN_NS_END
