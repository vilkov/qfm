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
#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include "../events_ns.h"


class QEvent;
class QMouseEvent;
class QKeyEvent;
class QFocusEvent;
class QContextMenuEvent;


EVENTS_NS_BEGIN

class EventHandler
{
public:
	virtual ~EventHandler() {}

	virtual bool mouseReleaseEvent(QMouseEvent *event) = 0;
	virtual bool mouseDoubleClickEvent(QMouseEvent *event) = 0;
	virtual bool keyboardEvent(QKeyEvent *event) = 0;
    virtual bool focusOutEvent(QFocusEvent *event) = 0;
    virtual bool focusInEvent(QFocusEvent *event) = 0;
    virtual bool contextMenuEvent(QContextMenuEvent *event) = 0;
};

EVENTS_NS_END

#endif /* EVENTHANDLER_H_ */
