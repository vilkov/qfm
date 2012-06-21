#ifndef APPLICATIONSETTINGSGENERALPAGE_H_
#define APPLICATIONSETTINGSGENERALPAGE_H_

#include <QtCore/QCoreApplication>
#include "../../tools/settings/options/gui/page/settingspage.h"
#include "../../tools/settings/options/gui/settingsvisibleoptionvalue.h"


SETTINGS_NS_BEGIN

class GeneralPage : public Page
{
	Q_DECLARE_TR_FUNCTIONS(GeneralPage)

public:
	GeneralPage(Option *parent);

private:
	VisibleOptionValue m_option;
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSGENERALPAGE_H_ */
