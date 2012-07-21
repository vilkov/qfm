#include "idmplugin.h"
#include "nodes/root/idmrootnode.h"
#include "../../application.h"


IDM_PLUGIN_NS_BEGIN

Plugin::Plugin(::Tools::Settings::Option *parentOption) :
	m_settings(parentOption)
{}

void Plugin::registered()
{
	Application::globalMenu()->registerAction(&m_createDbAction, ::DesktopEnvironment::ContextMenuFactory::SingleFolder);
}

::Tools::Settings::Page *Plugin::settings()
{
	return &m_settings;
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
	FileTypeId type;
	type.mime = QString::fromLatin1("inode/directory");

	return FileTypeIdList() << type;
}

Node *Plugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	IFileContainer::Holder folder(container->open(file, m_error));

	if (folder && folder->contains(fileName()))
		return new IdmRootNode(folder, parent);
	else
		return NULL;
}

const QString &Plugin::fileName()
{
	static const QString res = QString::fromLatin1(".storage.idm");
	return res;
}

IDM_PLUGIN_NS_END