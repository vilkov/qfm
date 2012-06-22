#include "applicationsettingsdialogsettings.h"


SETTINGS_NS_BEGIN

ApplicationDialogSettings::ApplicationDialogSettings(Option *parent) :
	DialogSettings(parent),
	m_font(QString::fromLatin1("Font"), this)
{
	manage(&m_font);
}

QFont ApplicationDialogSettings::font() const
{
	return m_font.font();
}

SETTINGS_NS_END
