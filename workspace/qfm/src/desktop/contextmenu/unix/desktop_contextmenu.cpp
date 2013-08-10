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
#include "../desktop_contextmenu.h"


DESKTOP_NS_BEGIN

ContextMenu::ContextMenu()
{
	m_menu.addAction(tr("System actions 1 (Create, etc.)"));
	m_menu.addSeparator();
	m_menu.addAction(tr("Copy/Paste"));
	m_menu.addSeparator();
	m_menu.addAction(tr("Open with..."));
	m_menu.addAction(tr("Plugins actions"));
	m_menu.addSeparator();
	m_menu.addAction(tr("Properties"));
}

DESKTOP_NS_END
