#ifndef KEYBOARDEVENTSOURCE_H_
#define KEYBOARDEVENTSOURCE_H_

#include "eventsourcebase.h"


template <typename BaseClass = EventSourceBase<QWidget> >
class KeyboardEventSource : public BaseClass
{
public:
	KeyboardEventSource(EventHandler *eventHandler, QWidget *parent = 0) :
		BaseClass(eventHandler, parent)
	{}

protected:
    virtual void keyPressEvent(QKeyEvent *event)
    {
    	if (!BaseClass::eventHandler()->keyboardEvent(event))
    		BaseClass::keyPressEvent(event);
    }
};

#endif /* KEYBOARDEVENTSOURCE_H_ */
