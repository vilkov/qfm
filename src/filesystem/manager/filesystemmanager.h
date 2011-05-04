#ifndef FILESYSTEMMANAGER_H_
#define FILESYSTEMMANAGER_H_

#include "../tree/filesystemtree.h"


class FileSystemManager
{
	Q_DISABLE_COPY(FileSystemManager)

public:
	class Plugin
	{
	public:
		virtual ~Plugin() {}

		virtual FileSystemTree *parse(const QString &absoluteFilePath) = 0;
	};


public:
	FileSystemManager();
	virtual ~FileSystemManager();

	virtual FileSystemTree *parse(const QString &absoluteFilePath) const;

	void registerStaticPlugin(Plugin *plugin) { m_staticPlugins.push_back(plugin); }
	void registerDynamicPlugin(Plugin *plugin) { m_dynamicPlugins.push_back(plugin); }

private:
	QList<Plugin*> m_staticPlugins;
	QList<Plugin*> m_dynamicPlugins;
};

#endif /* FILESYSTEMMANAGER_H_ */
