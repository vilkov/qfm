#include "applicationsettingsoptionfont.h"
#include "../../application.h"

#include <QtGui/QFontDialog>


SETTINGS_NS_BEGIN

AppllicationOptionFont::AppllicationOptionFont(const QString &label, const QString &id, Option *parent, const QFont &defaultValue) :
	VisibleOptioinFont(label, id, parent, defaultValue)
{}

bool AppllicationOptionFont::chooseFont(QFont &font) const
{
	bool ok;
	font = QFontDialog::getFont(&ok, font, Application::mainWindow());
	return ok;
}

SETTINGS_NS_END
