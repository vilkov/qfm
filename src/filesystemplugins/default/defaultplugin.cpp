#include "defaultplugin.h"
#include "defaultfoldernode.h"


DEFAULT_PLUGIN_NS_BEGIN

Plugin::Plugin() :
	m_settings()
{}

void Plugin::registered()
{}

const ::Tools::Settings::Tab *Plugin::settings() const
{
	return &m_settings;
}

Node *Plugin::node(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	IFileContainer::Holder folder(container->open(file->fileName(), false, m_error));

	if (folder)
		return new FolderNode(folder, parent);
	else
		return NULL;
}

DEFAULT_PLUGIN_NS_END
