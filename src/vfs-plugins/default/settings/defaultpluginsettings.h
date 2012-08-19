#ifndef DEFAULTPLUGINSETTINGS_H_
#define DEFAULTPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../tools/settings/options/gui/page/settingspage.h"
#include "../../../tools/settings/options/gui/decoration/settingsdecorationtext.h"


DEFAULT_PLUGIN_NS_BEGIN

class Settings : public ::Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings(Option *parent);

private:
	::Tools::Settings::DecorationText m_text;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPLUGINSETTINGS_H_ */
