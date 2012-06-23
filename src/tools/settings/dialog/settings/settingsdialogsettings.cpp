#include "settingsdialogsettings.h"


SETTINGS_NS_BEGIN

DialogSettings::DialogSettings(Option *parent) :
	WidgetScope(QString::fromLatin1("SettingsDialog"), parent),
	m_splitterGeometry(QString::fromLatin1("SplitterGeometry"), this)
{
	manage(&m_splitterGeometry);
}

SETTINGS_NS_END
