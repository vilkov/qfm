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

Node *Plugin::node(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	if (file->isDir())
	{
		IFileContainer::Holder folder(container->open(file->fileName(), false, m_error));

		if (folder)
		{
			static QString fileName = QString::fromLatin1(".storage.idm");

			if (folder->contains(fileName))
				return new IdmRootNode(folder->location(), parent);
		}
	}

	return NULL;
}

IDM_PLUGIN_NS_END
