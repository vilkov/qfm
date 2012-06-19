#ifndef APPLICATIONSETTINGSCONTAINER_H_
#define APPLICATIONSETTINGSCONTAINER_H_

#include <QtCore/QObject>
#include "../tools/settings/settingscontainer.h"


class SettingsContainer : public QObject, public Tools::Settings::Container
{
	Q_OBJECT

public:
	SettingsContainer(QObject *parent = 0);

	using Container::manage;
};

#endif /* APPLICATIONSETTINGSCONTAINER_H_ */
