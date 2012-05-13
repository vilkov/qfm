#ifndef IDMPLUGIN_H_
#define IDMPLUGIN_H_

#include <QtCore/QString>
#include "actions/idmcreatedbaction.h"
#include "settings/idmpluginsettings.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


IDM_PLUGIN_NS_BEGIN

class Plugin : public IFilePlugin
{
public:
	Plugin();

	/* IPlugin */
	virtual void registered();
	virtual const ::Tools::Settings::Tab *settings() const;

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

#endif /* IDMPLUGIN_H_ */
