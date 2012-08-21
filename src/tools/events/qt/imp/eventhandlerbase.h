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
    inline bool invokeMethod1(Method method, QEvent *event)
    {
		if (method)
		{
			(m_object->*method)();
			event->accept();
			return true;
		}
		else
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
