#ifndef SETTINGSDIALOGSETTINGS_H_
#define SETTINGSDIALOGSETTINGS_H_

#include <QtGui/QFont>
#include "../../options/settingswidgetscope.h"


SETTINGS_NS_BEGIN

class DialogSettings : public WidgetScope
{
public:
	DialogSettings(Option *parent);

	virtual QFont font() const = 0;
};

SETTINGS_NS_END

#endif /* SETTINGSDIALOGSETTINGS_H_ */
