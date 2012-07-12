#ifndef ARCPLUGINSETTINGS_H_
#define ARCPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../arcplugin_ns.h"
#include "../../../tools/settings/options/gui/page/settingspage.h"


ARC_PLUGIN_NS_BEGIN

class Settings : public Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings(Option *parent);
};

ARC_PLUGIN_NS_END

#endif /* ARCPLUGINSETTINGS_H_ */
