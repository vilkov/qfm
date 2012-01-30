#include "idmcreatedbaction.h"


IDM_PLUGIN_NS_BEGIN

CreateDbAction::CreateDbAction() :
	SyncFileAction(QIcon(), tr("Create database..."))
{}

void CreateDbAction::process(const FilesList &files) const
{

}

IDM_PLUGIN_NS_END
