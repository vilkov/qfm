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
#include "settingsvisiblecontainer.h"
#include "dialog/settingsdialog.h"


SETTINGS_NS_BEGIN

VisibleContainer::VisibleContainer(const QString &storage) :
	Container(storage)
{}

void VisibleContainer::manage(Page *option)
{
	Container::manage(option);
	m_pages.push_back(option);
}

void VisibleContainer::exec(const QString &title, DialogSettings &settings, QWidget *parent)
{
	Q_ASSERT(!m_pages.isEmpty());
	Dialog(title, settings, m_pages, parent).exec();
}

SETTINGS_NS_END
