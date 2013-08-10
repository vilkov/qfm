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
#ifndef FOCUSEVENTHANDLER_H_
#define FOCUSEVENTHANDLER_H_

#include <QtGui/QFocusEvent>
#include "eventhandlerbase.h"


EVENTS_NS_BEGIN

template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
	typename IntercepEvent = Templates::bool_value<false>
>
class FocusOutEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
	typedef typename BaseClass::Method   Method;

public:
	FocusOutEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(0)
	{}

	virtual bool focusOutEvent(QFocusEvent *event)
	{
		if (IntercepEvent::value)
			if (FallbackToBaseClass::value)
				return this->invokeMethod1(m_handler, event) ? true : BaseClass::focusOutEvent(event);
			else
				return this->invokeMethod1(m_handler, event);
		else
			if (FallbackToBaseClass::value)
				return this->invokeMethod2(m_handler, event) ? true : BaseClass::focusOutEvent(event);
			else
				return this->invokeMethod2(m_handler, event);
	}

	void registerFocusOutEventHandler(Method handler) { m_handler = handler; }
	void unregisterFocusOutEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};


template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
	typename IntercepEvent = Templates::bool_value<false>
>
class FocusInEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
	typedef typename BaseClass::Method   Method;

public:
	FocusInEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(0)
	{}

	virtual bool focusInEvent(QFocusEvent *event)
	{
		if (IntercepEvent::value)
			if (FallbackToBaseClass::value)
				return this->invokeMethod1(m_handler, event) ? true : BaseClass::focusInEvent(event);
			else
				return this->invokeMethod1(m_handler, event);
		else
			if (FallbackToBaseClass::value)
				return this->invokeMethod2(m_handler, event) ? true : BaseClass::focusInEvent(event);
			else
				return this->invokeMethod2(m_handler, event);
	}

	void registerFocusInEventHandler(Method handler) { m_handler = handler; }
	void unregisterFocusInEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};

EVENTS_NS_END

#endif /* FOCUSEVENTHANDLER_H_ */
