#ifndef APPLICATIONSETTINGSCONTAINER_H_
#define APPLICATIONSETTINGSCONTAINER_H_

#include <QtCore/QObject>
#include "pages/applicationsettingshelppage.h"
#include "pages/applicationsettingsgeneralpage.h"
#include "pages/applicationsettingspluginspage.h"
#include "dialog/applicationsettingsdialogsettings.h"
#include "../tools/settings/settingsvisiblecontainer.h"
#include "../tools/settings/options/value/settingsoptionvalue.h"


class SettingsContainer : public QObject, public Tools::Settings::VisibleContainer
{
	Q_OBJECT

public:
	SettingsContainer(QObject *parent = 0);

	const Tools::Settings::GeneralPage &generalPage() const { return m_generalPage; }
	Tools::Settings::GeneralPage &generalPage() { return m_generalPage; }

	const Tools::Settings::PluginsPage &pluginsPage() const { return m_pluginsPage; }
	Tools::Settings::PluginsPage &pluginsPage() { return m_pluginsPage; }

	using VisibleContainer::manage;
	void exec(QWidget *parent = 0);

private:
	Tools::Settings::GeneralPage m_generalPage;
	Tools::Settings::PluginsPage m_pluginsPage;
	Tools::Settings::HelpPage m_helpPage;

private:
	Tools::Settings::ApplicationDialogSettings m_dialogSettings;
};

#endif /* APPLICATIONSETTINGSCONTAINER_H_ */
