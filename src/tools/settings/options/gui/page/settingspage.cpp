#include "settingspage.h"


SETTINGS_NS_BEGIN

Page::Page(const QString &title, const QString &id, Option *parent) :
	GroupBox(title, id, parent)
{}

SETTINGS_NS_END
