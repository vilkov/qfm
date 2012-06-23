#ifndef APPLICATIONSETTINGSHELPPAGE_H_
#define APPLICATIONSETTINGSHELPPAGE_H_

#include <QtCore/QCoreApplication>
#include "../../tools/settings/options/gui/page/settingspage.h"


SETTINGS_NS_BEGIN

class HelpPage : public Page
{
	Q_DECLARE_TR_FUNCTIONS(HelpPage)

public:
	HelpPage(Option *parent);
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSHELPPAGE_H_ */
