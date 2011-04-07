#ifndef KEYBOARDEVENTHANDLER_H_
#define KEYBOARDEVENTHANDLER_H_

#include <QtCore/QMap>
#include <QtGui/QKeyEvent>
#include "eventhandlerbase.h"


template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
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
			return invokeMethod1(m_hotkeys.value(event->modifiers() + event->key(), 0), event);
		else
			return invokeMethod2(m_hotkeys.value(event->modifiers() + event->key(), 0), event);
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

#endif /* KEYBOARDEVENTHANDLER_H_ */
