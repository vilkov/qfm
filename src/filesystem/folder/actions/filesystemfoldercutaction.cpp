#include "filesystemfoldercutaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderCutAction::FolderCutAction() :
	m_action(Application::desktopService()->cutActionIcon(16), tr("Cut"), 0)
{
	m_action.setData(qVariantFromValue(static_cast<void *>(this)));
}

const QAction *FolderCutAction::action() const
{
	return &m_action;
}

void FolderCutAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
