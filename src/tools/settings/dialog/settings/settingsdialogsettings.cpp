#include "settingsdialogsettings.h"


SETTINGS_NS_BEGIN

DialogSettings::DialogSettings(Option *parent) :
	WidgetScope(QString::fromLatin1("SettingsDialog"), parent)
{}

SETTINGS_NS_END
