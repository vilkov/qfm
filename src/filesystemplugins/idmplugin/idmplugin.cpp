#include "idmplugin.h"
#include "nodes/root/idmrootnode.h"
#include "../../application.h"


IDM_PLUGIN_NS_BEGIN

Plugin::Plugin()
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
	return QString::fromLatin1("Idm");
}

void Plugin::registered()
{
	Application::globalMenu()->registerAction(&m_createDbAction, ::DesktopEnvironment::ContextMenuFactory::SingleFolder);
}

Node *Plugin::node(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	if (file->isDir())
	{
		IFileContainer::Holder folder(container->open(file->fileName(), false, m_error));

		if (folder && folder->contains(fileName()))
			return new IdmRootNode(folder, parent);
	}

	return NULL;
}

const QString &Plugin::fileName()
{
	static const QString res = QString::fromLatin1(".storage.idm");
	return res;
}

IDM_PLUGIN_NS_END
