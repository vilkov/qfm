#include "applicationsettingscontainer.h"


SettingsContainer::SettingsContainer(QObject *parent) :
	Container(storageLocation(QString::fromLatin1(".qfm")).append(QString::fromLatin1("/settings.xml")))
{}

void SettingsContainer::manage(Option *option)
{
	m_items.push_back(option);
}
