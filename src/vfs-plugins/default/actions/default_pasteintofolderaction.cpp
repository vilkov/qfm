#include "default_pasteintofolderaction.h"
#include <desktop/theme/desktop_theme.h>


DEFAULT_PLUGIN_NS_BEGIN

PasteIntoFolderAction::PasteIntoFolderAction() :
	AsyncFileAction(::Desktop::Theme::current()->pasteActionIcon(), tr("Paste into folder")),
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
