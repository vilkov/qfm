#include "settingsvisibleoptionhotkeys.h"


SETTINGS_NS_BEGIN

VisibleOptionHotKeys::VisibleOptionHotKeys(const QString &id, Option *parent) :
	Scope(id, parent)
{}

QLayout *VisibleOptionHotKeys::createEditor() const
{
	return NULL;
}

bool VisibleOptionHotKeys::accept() const
{
	return false;
}

void VisibleOptionHotKeys::reject() const
{

}

SETTINGS_NS_END
