#include "applicationsettingsdialogsettings.h"
#include "../../application.h"


SETTINGS_NS_BEGIN

ApplicationDialogSettings::ApplicationDialogSettings(Option *parent) :
	DialogSettings(parent)
{}

QFont ApplicationDialogSettings::font() const
{
	return Application::settings2()->generalPage().baseFont();
}

SETTINGS_NS_END
