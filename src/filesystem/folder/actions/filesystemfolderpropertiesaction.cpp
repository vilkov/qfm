#include "filesystemfolderpropertiesaction.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderPropertiesAction::FolderPropertiesAction() :
	m_action(Application::desktopService()->propertiesActionIcon(16), tr("Properties..."), 0)
{
	m_action.setData(qVariantFromValue(static_cast<void *>(this)));
}

const QAction *FolderPropertiesAction::action() const
{
	return &m_action;
}

void FolderPropertiesAction::process(const FilesList &files) const
{

}

FILE_SYSTEM_NS_END
