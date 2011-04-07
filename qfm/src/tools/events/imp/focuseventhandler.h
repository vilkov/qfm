#ifndef FOCUSEVENTHANDLER_H_
#define FOCUSEVENTHANDLER_H_

#include <QtGui/QFocusEvent>
#include "eventhandlerbase.h"


template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
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
			return invokeMethod1(m_handler, event);
		else
			return invokeMethod2(m_handler, event);
	}

	void registerFocusOutEventHandler(Method handler) { m_handler = handler; }
	void unregisterFocusOutEventHandler() { m_handler = 0;	}

private:
	Method m_handler;
};

#endif /* FOCUSEVENTHANDLER_H_ */
