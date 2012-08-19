#ifndef M3UPLUGINSETTINGS_H_
#define M3UPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../m3uplugin_ns.h"
#include "../../../tools/settings/options/gui/page/settingspage.h"


M3U_PLUGIN_NS_BEGIN

class Settings : public ::Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings(Option *parent);
};

M3U_PLUGIN_NS_END

#endif /* M3UPLUGINSETTINGS_H_ */
