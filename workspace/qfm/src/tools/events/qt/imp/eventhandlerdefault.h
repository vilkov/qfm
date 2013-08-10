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
#ifndef EVENTHANDLERDEFAULT_H_
#define EVENTHANDLERDEFAULT_H_

#include "eventhandlerbase.h"


EVENTS_NS_BEGIN

template <
	typename ListenerType,
	typename IntercepEvent = Templates::bool_value<true>
>
class EventHandlerDefault : public EventHandlerBase<ListenerType>
{
public:
	typedef EventHandlerBase<ListenerType> BaseClass;
	typedef typename BaseClass::Listener   Listener;
	typedef typename BaseClass::Method     Method;
	typedef bool (Listener::*DefaultMethod)(QEvent *event);

	enum Event
	{
		MouseReleaseEvent,
		MouseDoubleClickEvent,
		KeyboardEvent,
		FocusOutEvent,
		FocusInEvent,
		ContextMenuEvent,

		SizeOf_Event
	};

public:
	EventHandlerDefault(Listener *object) :
		BaseClass(object),
		m_handlers()
	{}

	void setDefaultHandler(Event event, DefaultMethod method) { m_handlers[event] = method; }

	virtual bool mouseReleaseEvent(QMouseEvent *event)
	{
		if (IntercepEvent::value)
			return invokeMethod3(m_handlers[MouseReleaseEvent], event);
		else
			return invokeMethod4(m_handlers[MouseReleaseEvent], event);
	}

	virtual bool mouseDoubleClickEvent(QMouseEvent *event)
	{
		if (IntercepEvent::value)
			return invokeMethod3(m_handlers[MouseDoubleClickEvent], event);
		else
			return invokeMethod4(m_handlers[MouseDoubleClickEvent], event);
	}

	virtual bool keyboardEvent(QKeyEvent *event)
	{
		if (IntercepEvent::value)
			return invokeMethod3(m_handlers[KeyboardEvent], event);
		else
			return invokeMethod4(m_handlers[KeyboardEvent], event);
	}

    virtual bool focusOutEvent(QFocusEvent *event)
    {
		if (IntercepEvent::value)
			return invokeMethod3(m_handlers[FocusOutEvent], event);
		else
			return invokeMethod4(m_handlers[FocusOutEvent], event);
    }

    virtual bool focusInEvent(QFocusEvent *event)
    {
		if (IntercepEvent::value)
			return invokeMethod3(m_handlers[FocusInEvent], event);
		else
			return invokeMethod4(m_handlers[FocusInEvent], event);
    }

    virtual bool contextMenuEvent(QContextMenuEvent *event)
    {
		if (IntercepEvent::value)
			return invokeMethod3(m_handlers[ContextMenuEvent], event);
		else
			return invokeMethod4(m_handlers[ContextMenuEvent], event);
    }

protected:
    inline bool invokeMethod3(DefaultMethod method, QEvent *event)
    {
		if (method && (BaseClass::listener()->*method)(event))
		{
			event->accept();
			return true;
		}

		return false;
    }
    inline bool invokeMethod4(DefaultMethod method, QEvent *event)
    {
		if (method)
			(BaseClass::listener()->*method)(event);

		return false;
    }

private:
    DefaultMethod m_handlers[SizeOf_Event];
};

EVENTS_NS_END

#endif /* EVENTHANDLERDEFAULT_H_ */
