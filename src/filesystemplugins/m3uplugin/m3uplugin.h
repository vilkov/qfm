#ifndef M3UPLUGIN_H_
#define M3UPLUGIN_H_

#include "settings/m3upluginsettings.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


M3U_PLUGIN_NS_BEGIN

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

private:
	Settings m_settings;
	QString m_identity;
};

M3U_PLUGIN_NS_END

#endif /* M3UPLUGIN_H_ */
