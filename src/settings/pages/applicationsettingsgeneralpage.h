#ifndef APPLICATIONSETTINGSGENERALPAGE_H_
#define APPLICATIONSETTINGSGENERALPAGE_H_

#include <QtCore/QCoreApplication>
#include "../options/applicationsettingsoptionfont.h"
#include "../../tools/settings/options/gui/page/settingspage.h"


SETTINGS_NS_BEGIN

class GeneralPage : public Page
{
	Q_DECLARE_TR_FUNCTIONS(GeneralPage)

public:
	GeneralPage(Option *parent);

	QFont baseFont() const { return m_baseFont.font(); }
	void setBaseFont(const QFont &value) { m_baseFont.setFont(value); }

private:
	AppllicationOptionFont m_baseFont;
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSGENERALPAGE_H_ */
