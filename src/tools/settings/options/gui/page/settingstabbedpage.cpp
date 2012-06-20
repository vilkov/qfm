#include "settingstabbedpage.h"


SETTINGS_NS_BEGIN

TabbedPage::TabbedPage(const QString &id, Option *parent) :
	Page(id, parent)
{}

QLayout *TabbedPage::createEditor() const
{
	return NULL;
}

bool TabbedPage::accept() const
{
	return false;
}

void TabbedPage::reject() const
{

}

SETTINGS_NS_END
