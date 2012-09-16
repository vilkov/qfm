#ifndef DEFAULT_PLUGIN_H_
#define DEFAULT_PLUGIN_H_

#include <vfs/interfaces/vfs_iplugin.h>
#include "settings/default_pluginsettings.h"


DEFAULT_PLUGIN_NS_BEGIN

class Plugin : public IContainerPlugin, public IFilePlugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);
	virtual ~Plugin();

	/* IPlugin */
	virtual void registered();
	virtual ::Tools::Settings::Page *settings();

	/* IContentPlugin */
	QString shema() const;
	virtual ::VFS::Node *open(const Uri::Iterator &path, QModelIndex &selected) const;

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual ::VFS::Node *open(const IFileContainer *container, const IFileInfo *file, ::VFS::Node *parent) const;

private:
	Settings m_settings;
	mutable QString m_error;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PLUGIN_H_ */
