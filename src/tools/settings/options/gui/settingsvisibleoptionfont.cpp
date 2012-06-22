#include "settingsvisibleoptionfont.h"

#include <QtGui/QFont>
#include <QtGui/QFontDialog>


SETTINGS_NS_BEGIN

VisibleOptioinFont::VisibleOptioinFont(const QString &id, Option *parent) :
	OptionFont(id, parent)
{}

QLayout *VisibleOptioinFont::createEditor(const QFont &font)
{
	return NULL;
}

bool VisibleOptioinFont::accept()
{
	return false;
}

void VisibleOptioinFont::reject()
{

}

QFont VisibleOptioinFont::chooseFont(QWidget *parent)
{
	bool ok;
	return QFontDialog::getFont(&ok, parent);
}

SETTINGS_NS_END
