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
#ifndef EVENTHANDLERBASE_H_
#define EVENTHANDLERBASE_H_

#include "../eventhandler.h"
#include "../../../templates/metatemplates.h"


EVENTS_NS_BEGIN

template <typename ListenerType>
class EventHandlerBase : public EventHandler
{
public:
	typedef ListenerType Listener;
	typedef void (Listener::*Method)();

public:
	EventHandlerBase(Listener *object) :
	    m_object(object)
	{
		Q_ASSERT(m_object != 0);
	}

	virtual bool mouseReleaseEvent(QMouseEvent *event) { return false; }
	virtual bool mouseDoubleClickEvent(QMouseEvent *event) { return false; }
	virtual bool keyboardEvent(QKeyEvent *event) { return false; }
    virtual bool focusOutEvent(QFocusEvent *event) { return false; }
    virtual bool focusInEvent(QFocusEvent *event) { return false; }
    virtual bool contextMenuEvent(QContextMenuEvent *event) { return false; }

protected:
    inline Listener *listener() const { return m_object; }

    inline bool invokeMethod1(Method method, QEvent *event)
    {
		if (method)
		{
			(m_object->*method)();
			event->accept();
			return true;
		}

		return false;
    }
    inline bool invokeMethod2(Method method, QEvent *event)
    {
		if (method)
			(m_object->*method)();

		return false;
    }

private:
    Listener *m_object;
};

EVENTS_NS_END

#endif /* EVENTHANDLERBASE_H_ */
