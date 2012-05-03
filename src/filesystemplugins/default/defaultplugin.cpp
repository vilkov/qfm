#include "defaultplugin.h"
#include "defaultfoldernode.h"


DEFAULT_PLUGIN_NS_BEGIN

Plugin::Plugin() :
	m_settings()
{}

void Plugin::beginGroup(const QString &name)
{

}

void Plugin::writeValue(const QString &name, const QVariant &value)
{

}

QVariant Plugin::readValue(const QString &name, const QVariant &defaultValue)
{
	return defaultValue;
}

void Plugin::endGroup()
{

}

QString Plugin::id() const
{
	return QString::fromLatin1("Default");
}

void Plugin::registered()
{}

Node *Plugin::node(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	IFileContainer::Holder folder(container->open(file->fileName(), false, m_error));

	if (folder)
		return new FolderNode(folder, parent);
	else
		return NULL;
}

DEFAULT_PLUGIN_NS_END
