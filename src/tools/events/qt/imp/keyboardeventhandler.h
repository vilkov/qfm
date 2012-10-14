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
#ifndef KEYBOARDEVENTHANDLER_H_
#define KEYBOARDEVENTHANDLER_H_

#include <QtCore/QMap>
#include <QtGui/QKeyEvent>
#include "eventhandlerbase.h"


EVENTS_NS_BEGIN

template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
	typename FallbackToBaseClass = Templates::bool_value<false>,
	typename IntercepEvent = Templates::bool_value<true>
>
class KeyboardEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
	typedef typename BaseClass::Method   Method;

public:
	KeyboardEventHandler(Listener *object) :
		BaseClass(object)
	{}

	virtual bool keyboardEvent(QKeyEvent *event)
	{
		if (IntercepEvent::value)
			if (FallbackToBaseClass::value)
				return invokeMethod1(m_hotkeys.value(event->modifiers() + event->key(), NULL), event) ? true : BaseClass::keyboardEvent(event);
			else
				return invokeMethod1(m_hotkeys.value(event->modifiers() + event->key(), NULL), event);
		else
			if (FallbackToBaseClass::value)
				return invokeMethod2(m_hotkeys.value(event->modifiers() + event->key(), NULL), event) ? true : BaseClass::keyboardEvent(event);
			else
				return invokeMethod2(m_hotkeys.value(event->modifiers() + event->key(), NULL), event);
	}

	void registerShortcut(quint32 modifier, Qt::Key key, Method method)
	{
		m_hotkeys[modifier + key] = method;
	}
	void registerShortcut(Qt::Modifier modifier, Qt::Key key, Method method)
	{
		m_hotkeys[modifier + key] = method;
	}
	void registerShortcut(Qt::KeyboardModifier modifier, Qt::Key key, Method method)
	{
		m_hotkeys[modifier + key] = method;
	}

	void unregisterShortcut(quint32 modifier, Qt::Key key)
	{
		m_hotkeys.remove(modifier + key);
	}
	void unregisterShortcut(Qt::Modifier modifier, Qt::Key key)
	{
		m_hotkeys.remove(modifier + key);
	}
	void unregisterShortcut(Qt::KeyboardModifier modifier, Qt::Key key)
	{
		m_hotkeys.remove(modifier + key);
	}

private:
	QMap<quint32, Method> m_hotkeys;
};

EVENTS_NS_END

#endif /* KEYBOARDEVENTHANDLER_H_ */
