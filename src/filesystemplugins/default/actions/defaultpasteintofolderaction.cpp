#include "defaultpasteintofolderaction.h"
#include "../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

PasteIntoFolderAction::PasteIntoFolderAction() :
	AsyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste into folder")),
	m_lockReason(tr("Copying..."))
{}

const QString &PasteIntoFolderAction::lockReason() const
{
	return m_lockReason;
}

PerformActionTask *PasteIntoFolderAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return NULL;
}

DEFAULT_PLUGIN_NS_END
