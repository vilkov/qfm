#ifndef IDM_PLUGIN_H_
#define IDM_PLUGIN_H_

#include <QtCore/QString>
#include <vfs/interfaces/vfs_iplugin.h>
#include "actions/idm_createdbaction.h"
#include "settings/idm_pluginsettings.h"


IDM_PLUGIN_NS_BEGIN

class Plugin : public IFilePlugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual void registered();
	virtual ::Tools::Settings::Page *settings();

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

	static const QString &fileName();

private:
	Settings m_settings;
	mutable QString m_error;
	CreateDbAction m_createDbAction;
};

IDM_PLUGIN_NS_END

#endif /* IDM_PLUGIN_H_ */
