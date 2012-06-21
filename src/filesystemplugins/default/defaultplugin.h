#ifndef DEFAULTPLUGIN_H_
#define DEFAULTPLUGIN_H_

#include "settings/defaultpluginsettings.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


DEFAULT_PLUGIN_NS_BEGIN

class Plugin : public IContainerPlugin, public IFilePlugin
{
public:
	Plugin();
	virtual ~Plugin();

	/* IPlugin */
	virtual void registered();
//	virtual const ::Tools::Settings::Tab *settings() const;

	/* IContentPlugin */
	QString shema() const;
	virtual Node *open(const Path::Iterator &path, QModelIndex &selected) const;

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	Settings m_settings;
	mutable QString m_error;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPLUGIN_H_ */
