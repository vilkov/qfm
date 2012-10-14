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
#include "pathedit.h"
#include <QKeyEvent>


PathEdit::PathEdit(::Tools::Events::EventHandler *eventHandler, QWidget *parent) :
	QLineEdit(parent),
	m_eventHandler(eventHandler)
{
	Q_ASSERT(m_eventHandler != 0);
}

void PathEdit::startEdit()
{
	m_backupText = text();
	setFocus();
	selectAll();
}

void PathEdit::cancelEdit()
{
	setText(m_backupText);
	end(false);
}

void PathEdit::mouseReleaseEvent(QMouseEvent *event)
{
	if (!m_eventHandler->mouseReleaseEvent(event))
		QLineEdit::mouseReleaseEvent(event);
}

void PathEdit::keyPressEvent(QKeyEvent *event)
{
	if (!m_eventHandler->keyboardEvent(event))
		QLineEdit::keyPressEvent(event);
}

void PathEdit::focusOutEvent(QFocusEvent *event)
{
	if (!m_eventHandler->focusOutEvent(event))
		QLineEdit::focusOutEvent(event);
}
