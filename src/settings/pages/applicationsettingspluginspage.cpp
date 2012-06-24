#include "applicationsettingspluginspage.h"


SETTINGS_NS_BEGIN

PluginsPage::PluginsPage(Option *parent) :
	Page(tr("Plugins"), QString::fromLatin1("Plugins"), parent),
	m_text(tr("This section contains information and preferences of installed plugins."), this)
{
	manage(&m_text, &m_text);
}

SETTINGS_NS_END
