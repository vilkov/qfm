#include "settingsdialogsettings.h"


SETTINGS_NS_BEGIN

DialogSettings::DialogSettings(Option *parent) :
	WidgetScope(QString::fromLatin1("SettingsDialog"), parent),
	m_splitterState(QString::fromLatin1("SplitterState"), this)
{
	manage(&m_splitterState);
}

SETTINGS_NS_END
