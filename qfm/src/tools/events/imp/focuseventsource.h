#ifndef FOCUSEVENTSOURCE_H_
#define FOCUSEVENTSOURCE_H_

#include "eventsourcebase.h"


template <typename BaseClass = EventSourceBase<QWidget> >
class FocusOutEventSource : public BaseClass
{
public:
	FocusOutEventSource(EventHandler *eventHandler, QWidget *parent = 0) :
		BaseClass(eventHandler, parent)
	{}

protected:
    virtual void focusOutEvent(QFocusEvent *event)
    {
    	if (!BaseClass::eventHandler()->focusOutEvent(event))
    		BaseClass::focusOutEvent(event);
    }
};

#endif /* FOCUSEVENTSOURCE_H_ */
