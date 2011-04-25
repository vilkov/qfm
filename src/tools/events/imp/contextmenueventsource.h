#ifndef CONTEXTMENUEVENTSOURCE_H_
#define CONTEXTMENUEVENTSOURCE_H_

#include "eventsourcebase.h"


template <typename BaseClass = EventSourceBase<QWidget> >
class ContextMenuEventSource : public BaseClass
{
public:
	ContextMenuEventSource(EventHandler *eventHandler, QWidget *parent = 0) :
		BaseClass(eventHandler, parent)
	{}

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event)
    {
    	if (!BaseClass::eventHandler()->contextMenuEvent(event))
    		BaseClass::contextMenuEvent(event);
    }
};

#endif /* CONTEXTMENUEVENTSOURCE_H_ */
