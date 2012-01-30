#include "idmplugin.h"
#include "nodes/root/idmrootnode.h"
#include "../../application.h"


IDM_PLUGIN_NS_BEGIN

Plugin::Plugin()
{}

void Plugin::registered()
{
	Application::globalMenu()->registerAction(&m_createDbAction, ::DesktopEnvironment::ContextMenuFactory::SingleFolder);
}

Node *Plugin::node(const IFileInfo *info, Node *parent) const
{
	if (info->absoluteFilePath() == QString::fromLatin1("/home/dav/idm"))
		return new IdmRootNode(info->absoluteFilePath(QString::fromLatin1(".storage.idm")), parent);
	else
		return 0;
}

IDM_PLUGIN_NS_END
