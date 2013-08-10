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
#include "contextmenu.h"
#ifdef Q_WS_WIN
//#	include "win/contextmenu_win.h"
#endif
#include <QtGui/QCursor>


ContextMenu::ContextMenu(QWidget *parent) :
#ifdef Q_WS_WIN
	m_data()//new ContextMenuWin(parent))
#else
	m_data()
#endif
{}

void ContextMenu::popup(const QString &parentDir, const QStringList &files)
{
//	m_data->popup(parentDir, files, QCursor::pos());
}
