#include "filesystempluginsmanager.h"
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include "../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

PluginsManager::PluginsManager()
{

}

PluginsManager::~PluginsManager()
{
	qDeleteAll(m_dynamicPlugins);
}

Node *PluginsManager::node(const IFileControl *control, Node *parent) const
{
	Node *res = 0;

	if (control->isFile())
	{
		QString error;
		PScopedPointer<IFile> file(control->open(IFile::ReadOnly, error));

		if (file)
		{
			for (PluginsList::size_type i = 0, size = m_staticPlugins.size(); i < size; ++i)
				if (!file->seek(0))
					return 0;
				else
					if (res = m_staticPlugins.at(i)->node(control, file.data(), parent))
						break;

			if (res == 0)
				for (PluginsList::size_type i = 0, size = m_dynamicPlugins.size(); i < size; ++i)
					if (!file->seek(0))
						return 0;
					else
						if (res = m_dynamicPlugins.at(i)->node(control, file.data(), parent))
							break;
		}
	}
	else
		if (control->isDir())
		{
			for (PluginsList::size_type i = 0, size = m_staticPlugins.size(); i < size; ++i)
				if (res = m_staticPlugins.at(i)->node(control, parent))
					break;

			if (res == 0)
				for (PluginsList::size_type i = 0, size = m_dynamicPlugins.size(); i < size; ++i)
					if (res = m_dynamicPlugins.at(i)->node(control, parent))
						break;
		}

	return res;
}

FILE_SYSTEM_NS_END
