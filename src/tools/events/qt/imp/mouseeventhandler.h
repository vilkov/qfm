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
				return invokeMethod1(m_handler, event) ? true : BaseClass::mouseReleaseEvent(event);
			else
				return invokeMethod1(m_handler, event);
		else
			if (FallbackToBaseClass::value)
				return invokeMethod2(m_handler, event) ? true : BaseClass::mouseReleaseEvent(event);
			else
				return invokeMethod2(m_handler, event);
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
			return invokeMethod1(m_handler, event) ? true : BaseClass::mouseDoubleClickEvent(event);
		else
			return invokeMethod2(m_handler, event) ? true : BaseClass::mouseDoubleClickEvent(event);
	}

	void registerMouseDoubleClickEventHandler(Method handler) { m_handler = handler; }
	void unregisterMouseDoubleClickEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};

EVENTS_NS_END

#endif /* MOUSEEVENTHANDLER_H_ */
