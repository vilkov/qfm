#ifndef APPLICATIONSETTINGSDIALOGSETTINGS_H_
#define APPLICATIONSETTINGSDIALOGSETTINGS_H_

#include "../../tools/settings/dialog/settings/settingsdialogsettings.h"
#include "../../tools/settings/options/value/settingsoptionfont.h"


SETTINGS_NS_BEGIN

class ApplicationDialogSettings : public DialogSettings
{
public:
	ApplicationDialogSettings(Option *parent);

	virtual QFont font() const;
	void setFont(const QFont &font) { m_font.setFont(font); }

private:
	OptionFont m_font;
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSDIALOGSETTINGS_H_ */
