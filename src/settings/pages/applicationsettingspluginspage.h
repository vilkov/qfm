#ifndef APPLICATIONSETTINGSPLUGINSPAGE_H_
#define APPLICATIONSETTINGSPLUGINSPAGE_H_

#include <QtCore/QCoreApplication>
#include "../../tools/settings/options/gui/page/settingspage.h"
#include "../../tools/settings/options/gui/decoration/settingsdecorationtext.h"


SETTINGS_NS_BEGIN

class PluginsPage : public Page
{
	Q_DECLARE_TR_FUNCTIONS(PluginsPage)

public:
	PluginsPage(Option *parent);

private:
	DecorationText m_text;
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSPLUGINSPAGE_H_ */
