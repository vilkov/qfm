#include "applicationsettingsgeneralpage.h"
#include "../../application.h"


SETTINGS_NS_BEGIN

GeneralPage::GeneralPage(Option *parent) :
	Page(tr("General"), QString::fromLatin1("General"), parent),
	m_baseFont(tr("Base font"), QString::fromLatin1("BaseFont"), this, Application::font())
{
	manage(&m_baseFont, &m_baseFont);
}

SETTINGS_NS_END
