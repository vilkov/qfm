#include "settingspage.h"


SETTINGS_NS_BEGIN

Page::Page(const QString &id, Option *parent) :
	Scope(id, parent)
{}

QLayout *Page::createEditor() const
{
	return NULL;
}

bool Page::accept() const
{
	return false;
}

void Page::reject() const
{

}

SETTINGS_NS_END
