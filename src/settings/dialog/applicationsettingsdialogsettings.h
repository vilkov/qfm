#ifndef APPLICATIONSETTINGSDIALOGSETTINGS_H_
#define APPLICATIONSETTINGSDIALOGSETTINGS_H_

#include "../../tools/settings/dialog/settings/settingsdialogsettings.h"


SETTINGS_NS_BEGIN

class ApplicationDialogSettings : public DialogSettings
{
public:
	ApplicationDialogSettings(Option *parent);

	virtual QFont font() const;
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSDIALOGSETTINGS_H_ */
