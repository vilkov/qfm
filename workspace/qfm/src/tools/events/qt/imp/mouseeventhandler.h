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
#ifndef MOUSEEVENTHANDLER_H_
#define MOUSEEVENTHANDLER_H_

#include <QtGui/QMouseEvent>
#include "eventhandlerbase.h"


EVENTS_NS_BEGIN

template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
	typename IntercepEvent = Templates::bool_value<false>
>
class MouseReleaseEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
	typedef typename BaseClass::Method   Method;

public:
	MouseReleaseEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(0)
	{}

	virtual bool mouseReleaseEvent(QMouseEvent *event)
	{
		if (IntercepEvent::value)
			if (FallbackToBaseClass::value)
				return this->invokeMethod1(m_handler, event) ? true : BaseClass::mouseReleaseEvent(event);
			else
				return this->invokeMethod1(m_handler, event);
		else
			if (FallbackToBaseClass::value)
				return this->invokeMethod2(m_handler, event) ? true : BaseClass::mouseReleaseEvent(event);
			else
				return this->invokeMethod2(m_handler, event);
	}

	void registerMouseReleaseEventHandler(Method handler) { m_handler = handler; }
	void unregisterMouseReleaseEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};


template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
	typename IntercepEvent = Templates::bool_value<true>
>
class MouseDoubleClickEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
	typedef typename BaseClass::Method   Method;

public:
	MouseDoubleClickEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(0)
	{}

	virtual bool mouseDoubleClickEvent(QMouseEvent *event)
	{
		if (IntercepEvent::value)
			return this->invokeMethod1(m_handler, event) ? true : BaseClass::mouseDoubleClickEvent(event);
		else
			return this->invokeMethod2(m_handler, event) ? true : BaseClass::mouseDoubleClickEvent(event);
	}

	void registerMouseDoubleClickEventHandler(Method handler) { m_handler = handler; }
	void unregisterMouseDoubleClickEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};

EVENTS_NS_END

#endif /* MOUSEEVENTHANDLER_H_ */
