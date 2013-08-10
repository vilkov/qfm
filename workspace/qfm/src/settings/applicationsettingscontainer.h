/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef APPLICATIONSETTINGSCONTAINER_H_
#define APPLICATIONSETTINGSCONTAINER_H_

#include <QtCore/QObject>
#include <tools/settings/settingsvisiblecontainer.h>
#include <tools/settings/options/value/settingsoptionvalue.h>
#include "pages/applicationsettingshelppage.h"
#include "pages/applicationsettingsgeneralpage.h"
#include "pages/applicationsettingspluginspage.h"
#include "dialog/applicationsettingsdialogsettings.h"


class SettingsContainer : public QObject, public ::Tools::Settings::VisibleContainer
{
	Q_OBJECT

public:
	SettingsContainer(QObject *parent = 0);

	const ::Tools::Settings::GeneralPage &generalPage() const { return m_generalPage; }
	::Tools::Settings::GeneralPage &generalPage() { return m_generalPage; }

	const ::Tools::Settings::PluginsPage &pluginsPage() const { return m_pluginsPage; }
	::Tools::Settings::PluginsPage &pluginsPage() { return m_pluginsPage; }

	using VisibleContainer::manage;
	void exec(QWidget *parent = 0);

private:
	::Tools::Settings::GeneralPage m_generalPage;
	::Tools::Settings::PluginsPage m_pluginsPage;
	::Tools::Settings::HelpPage m_helpPage;

private:
	::Tools::Settings::ApplicationDialogSettings m_dialogSettings;
};

#endif /* APPLICATIONSETTINGSCONTAINER_H_ */
