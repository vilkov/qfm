#include "filesystemfolderpasteclipboardaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteClipboardAction::FolderPasteClipboardAction() :
	m_action(Application::desktopService()->pasteActionIcon(16), tr("Paste from clipboard"), 0)
{
	m_action.setData(qVariantFromValue(static_cast<void *>(this)));
}

const QAction *FolderPasteClipboardAction::action() const
{
	return &m_action;
}

void FolderPasteClipboardAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
