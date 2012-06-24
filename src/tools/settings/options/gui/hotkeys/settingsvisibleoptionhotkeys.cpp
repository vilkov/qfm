#include "settingsvisibleoptionhotkeys.h"


SETTINGS_NS_BEGIN

VisibleOptionHotKeys::VisibleOptionHotKeys(const QString &id, Option *parent) :
	Scope(id, parent)
{}

QWidget *VisibleOptionHotKeys::createEditor()
{
	return NULL;
}

bool VisibleOptionHotKeys::accept()
{
	return false;
}

void VisibleOptionHotKeys::reject()
{

}

SETTINGS_NS_END
