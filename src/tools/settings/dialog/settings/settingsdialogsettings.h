#ifndef SETTINGSDIALOGSETTINGS_H_
#define SETTINGSDIALOGSETTINGS_H_

#include "../../options/settingswidgetscope.h"


SETTINGS_NS_BEGIN

class DialogSettings : public WidgetScope
{
public:
	DialogSettings(Option *parent);
};

SETTINGS_NS_END

#endif /* SETTINGSDIALOGSETTINGS_H_ */
