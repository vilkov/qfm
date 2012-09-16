#ifndef IDM_PLUGINSETTINGS_H_
#define IDM_PLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../idm_ns.h"
#include "../../../tools/settings/options/gui/page/settingspage.h"


IDM_PLUGIN_NS_BEGIN

class Settings : public ::Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings(Option *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDM_PLUGINSETTINGS_H_ */
