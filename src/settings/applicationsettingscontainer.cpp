#include "applicationsettingscontainer.h"


SettingsContainer::SettingsContainer(QObject *parent) :
	VisibleContainer(storageLocation(QString::fromLatin1(".qfm")).append(QString::fromLatin1("/settings.xml"))),
	m_dialogSettings(this),
	m_generalPage(this)
{
	manage(&m_dialogSettings);
	manage(&m_generalPage);
}

void SettingsContainer::exec(QWidget *parent)
{
	VisibleContainer::exec(tr("Settings"), m_dialogSettings, parent);
}
