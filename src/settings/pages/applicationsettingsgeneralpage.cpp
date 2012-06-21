#include "applicationsettingsgeneralpage.h"


SETTINGS_NS_BEGIN

GeneralPage::GeneralPage(Option *parent) :
	Page(tr("General"), QString::fromLatin1("General"), parent),
	m_option(tr("Just some test"), QString::fromLatin1("Test1"), this, QString::fromLatin1("Default value"))
{
	manage(&m_option, &m_option);
}

SETTINGS_NS_END
