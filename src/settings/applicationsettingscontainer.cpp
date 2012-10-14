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
