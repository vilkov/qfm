#include "filesystempluginsmanager.h"
#include <QtCore/QStringList>
#include <QtCore/QFile>


FILE_SYSTEM_NS_BEGIN

PluginsManager::PluginsManager()
{

}

PluginsManager::~PluginsManager()
{
	qDeleteAll(m_dynamicFilePlugins);
	qDeleteAll(m_dynamicFoldersPlugins);
}

Node *PluginsManager::node(const IFileInfo *info, Node *parent) const
{
	Node *res = 0;

	if (info->isFile())
	{
		QString error;
		QScopedPointer<IFile> file(info->open(IFileInfo::ReadOnly, error));

		if (file)
		{
			for (PluginsList::size_type i = 0, size = m_staticFilePlugins.size(); i < size; ++i)
				if (res = m_staticFilePlugins.at(i)->node(info, file.data(), parent))
					break;

			if (res == 0)
				for (PluginsList::size_type i = 0, size = m_dynamicFilePlugins.size(); i < size; ++i)
					if (res = m_dynamicFilePlugins.at(i)->node(info, file.data(), parent))
						break;
		}
	}
	else
		if (info->isDir())
			if (FolderPlugin *plugin = m_staticFoldersPlugins.value(info->absoluteFilePath(), 0))
				res = plugin->node(info, parent);

	return res;
}

FILE_SYSTEM_NS_END
