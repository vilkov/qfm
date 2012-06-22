#ifndef APPLICATIONSETTINGSOPTIONFONT_H_
#define APPLICATIONSETTINGSOPTIONFONT_H_

#include "../../tools/settings/options/gui/settingsvisibleoptionfont.h"


SETTINGS_NS_BEGIN

class AppllicationOptionFont : public VisibleOptioinFont
{
public:
	AppllicationOptionFont(const QString &label, const QString &id, Option *parent, const QFont &defaultValue);

protected:
	virtual bool chooseFont(QFont &font) const;
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSOPTIONFONT_H_ */
