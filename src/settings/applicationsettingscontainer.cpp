#include "applicationsettingscontainer.h"


SettingsContainer::SettingsContainer(QObject *parent) :
	Container(storageLocation(QString::fromLatin1(".qfm")).append(QString::fromLatin1("/settings.xml")))
{}
