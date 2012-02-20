#include "filesystemfolderpasteaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPasteAction::FolderPasteAction() :
	AsyncFileAction(Application::desktopService()->pasteActionIcon(16), tr("Paste")),
	m_lockReason(tr("Copying..."))
{}

const QString &FolderPasteAction::lockReason() const
{
	return m_lockReason;
}

PerformActionTask *FolderPasteAction::process(TasksNode *receiver, const FilesList &files) const
{
	return NULL;
}

FILE_SYSTEM_NS_END
