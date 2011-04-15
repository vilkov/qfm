#ifndef MOUSEEVENTSOURCE_H_
#define MOUSEEVENTSOURCE_H_

#include "eventsourcebase.h"


template <typename BaseClass = EventSourceBase<QWidget> >
class MouseReleaseEventSource : public BaseClass
{
public:
	MouseReleaseEventSource(EventHandler *eventHandler, QWidget *parent = 0) :
		BaseClass(eventHandler, parent)
	{}

protected:
	virtual void mouseReleaseEvent(QMouseEvent *event)
    {
    	if (!BaseClass::eventHandler()->mouseReleaseEvent(event))
    		BaseClass::mouseReleaseEvent(event);
    }
};


template <typename BaseClass = EventSourceBase<QWidget> >
class MouseDoubleClickEventSource : public BaseClass
{
public:
	MouseDoubleClickEventSource(EventHandler *eventHandler, QWidget *parent = 0) :
		BaseClass(eventHandler, parent)
	{}

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event)
    {
    	if (!BaseClass::eventHandler()->mouseDoubleClickEvent(event))
    		BaseClass::mouseDoubleClickEvent(event);
    }
};

#endif /* MOUSEEVENTSOURCE_H_ */
