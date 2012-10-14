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
#include "taskview.h"


TaskView::TaskView(QWidget *parent) :
	QWidget(parent),
	m_eventHandler(this),
	m_layout(this),
	m_text(this),
	m_cancel(&m_eventHandler, this)
{
	setLayout(&m_layout);
	m_layout.addWidget(&m_text);
	m_layout.addWidget(&m_cancel);

	m_eventHandler.registerMouseReleaseEventHandler(&TaskView::deleteLater);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Return, &TaskView::deleteLater);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Enter,  &TaskView::deleteLater);
	m_eventHandler.registerShortcut(Qt::NoModifier, Qt::Key_Space,  &TaskView::deleteLater);
}
