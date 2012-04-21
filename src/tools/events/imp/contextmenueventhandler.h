#ifndef CONTEXTMENUEVENTHANDLER_H_
#define CONTEXTMENUEVENTHANDLER_H_

#include <QtGui/QContextMenuEvent>
#include "eventhandlerbase.h"


template <
	typename BaseClass     = EventHandlerBase<Templates::null_type>,
	typename IntercepEvent = Templates::bool_value<true>
>
class ContextMenuEventHandler : public BaseClass
{
public:
	typedef typename BaseClass::Listener Listener;
	typedef typename BaseClass::Method   Method;

public:
	ContextMenuEventHandler(Listener *object) :
		BaseClass(object),
		m_handler(0)
	{}

    virtual bool contextMenuEvent(QContextMenuEvent *event)
    {
		if (IntercepEvent::value)
			return invokeMethod1(m_handler, event);
		else
			return invokeMethod2(m_handler, event);
    }

	void registerContextMenuEventHandler(Method handler) { m_handler = handler; }
	void unregisterContextMenuEventHandler() { m_handler = 0; }

private:
	Method m_handler;
};

#endif /* CONTEXTMENUEVENTHANDLER_H_ */
