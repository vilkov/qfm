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

		/**
		 * FIXME: Idm plugin supports only "Info" container.
		 */
		if (folder && folder->contains(fileName()))
			return new IdmRootNode(Info(folder->location(), Info::Refresh()), fileName(), parent);
	}

	return NULL;
}

const QString &Plugin::fileName()
{
	static const QString res = QString::fromLatin1(".storage.idm");
	return res;
}

IDM_PLUGIN_NS_END
