#ifndef IDMPLUGINSETTINGS_H_
#define IDMPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../idmplugin_ns.h"
#include "../../../tools/settings/options/gui/page/settingspage.h"


IDM_PLUGIN_NS_BEGIN

class Settings : public Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings(Option *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDMPLUGINSETTINGS_H_ */
