#include "defaultpluginsettings.h"


DEFAULT_PLUGIN_NS_BEGIN

Settings::Settings(Option *parent) :
	Page(tr("Default"), QString::fromLatin1("Default"), parent),
	m_text(tr("This plugin is the default embedded plugin which can handle regular folders and files."))
{
	manage(&m_text);
}

DEFAULT_PLUGIN_NS_END
