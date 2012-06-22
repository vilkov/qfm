#ifndef APPLICATIONSETTINGSCONTAINER_H_
#define APPLICATIONSETTINGSCONTAINER_H_

#include <QtCore/QObject>
#include "pages/applicationsettingsgeneralpage.h"
#include "dialog/applicationsettingsdialogsettings.h"
#include "../tools/settings/settingsvisiblecontainer.h"
#include "../tools/settings/options/value/settingsoptionvalue.h"


class SettingsContainer : public QObject, public Tools::Settings::VisibleContainer
{
	Q_OBJECT

public:
	SettingsContainer(QObject *parent = 0);

	using VisibleContainer::manage;
	void exec(QWidget *parent = 0);

private:
	Tools::Settings::ApplicationDialogSettings m_dialogSettings;
	Tools::Settings::GeneralPage m_generalPage;
};

#endif /* APPLICATIONSETTINGSCONTAINER_H_ */
