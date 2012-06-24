#include "settingstabbedpage.h"


SETTINGS_NS_BEGIN

TabbedPage::TabbedPage(const QString &title, const QString &id, Option *parent) :
	Page(title, id, parent)
{}

QWidget *TabbedPage::createEditor()
{
	return NULL;
}

bool TabbedPage::accept()
{
	return false;
}

void TabbedPage::reject()
{

}

SETTINGS_NS_END
