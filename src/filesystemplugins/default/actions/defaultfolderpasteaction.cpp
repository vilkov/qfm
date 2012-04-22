#include "defaultfolderpasteaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

FolderPasteAction::FolderPasteAction() :
	AsyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste")),
	m_lockReason(tr("Copying..."))
{}

const QString &FolderPasteAction::lockReason() const
{
	return m_lockReason;
}

PerformActionTask *FolderPasteAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return NULL;
}

DEFAULT_PLUGIN_NS_END
