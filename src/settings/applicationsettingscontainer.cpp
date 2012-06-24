#include "applicationsettingscontainer.h"


SettingsContainer::SettingsContainer(QObject *parent) :
	VisibleContainer(storageLocation(QString::fromLatin1(".qfm")).append(QString::fromLatin1("/settings.xml"))),
	m_generalPage(this),
	m_pluginsPage(this),
	m_helpPage(this),
	m_dialogSettings(this)
{
	manage(&m_generalPage);
	manage(&m_pluginsPage);
	manage(&m_helpPage);
	manage(&m_dialogSettings);
}

void SettingsContainer::exec(QWidget *parent)
{
	VisibleContainer::exec(tr("Settings"), m_dialogSettings, parent);
}
