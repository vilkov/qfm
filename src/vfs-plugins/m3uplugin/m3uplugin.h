#ifndef M3UPLUGIN_H_
#define M3UPLUGIN_H_

#include <vfs/interfaces/vfs_iplugin.h>
#include "settings/m3upluginsettings.h"


M3U_PLUGIN_NS_BEGIN

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

private:
	Settings m_settings;
};

M3U_PLUGIN_NS_END

#endif /* M3UPLUGIN_H_ */
