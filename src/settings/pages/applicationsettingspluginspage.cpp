#include "applicationsettingspluginspage.h"


SETTINGS_NS_BEGIN

PluginsPage::PluginsPage(Option *parent) :
	Page(tr("Plugins"), QString::fromLatin1("Plugins"), parent),
	m_text(tr("This section contains information and preferences of installed plugins."))
{
	manage(&m_text);
}

SETTINGS_NS_END
