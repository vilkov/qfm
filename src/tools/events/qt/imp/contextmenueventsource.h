#ifndef CONTEXTMENUEVENTSOURCE_H_
#define CONTEXTMENUEVENTSOURCE_H_

#include "eventsourcebase.h"


EVENTS_NS_BEGIN

#include "../../../arguments/define.h"

template <typename BaseClass = EventSourceBase<QWidget> >
class ContextMenuEventSource : public BaseClass
{
public:
	ContextMenuEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <TEMPLATE_ARGS_1>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_1) :
		BaseClass(eventHandler, VALUES_1)
	{}

    template <TEMPLATE_ARGS_2>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_2) :
		BaseClass(eventHandler, VALUES_2)
	{}

    template <TEMPLATE_ARGS_3>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_3) :
		BaseClass(eventHandler, VALUES_3)
	{}

    template <TEMPLATE_ARGS_4>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_4) :
		BaseClass(eventHandler, VALUES_4)
	{}

    template <TEMPLATE_ARGS_5>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_5) :
		BaseClass(eventHandler, VALUES_5)
	{}

    template <TEMPLATE_ARGS_6>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_6) :
		BaseClass(eventHandler, VALUES_6)
	{}

    template <TEMPLATE_ARGS_7>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_7) :
		BaseClass(eventHandler, VALUES_7)
	{}

    template <TEMPLATE_ARGS_8>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_8) :
		BaseClass(eventHandler, VALUES_8)
	{}

    template <TEMPLATE_ARGS_9>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_9) :
		BaseClass(eventHandler, VALUES_9)
	{}

    template <TEMPLATE_ARGS_10>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_10) :
		BaseClass(eventHandler, VALUES_10)
	{}


protected:
    virtual void contextMenuEvent(QContextMenuEvent *event)
    {
    	if (!BaseClass::eventHandler()->contextMenuEvent(event))
    		BaseClass::contextMenuEvent(event);
    }
};

#include "../../../arguments/undefine.h"

EVENTS_NS_END

#endif /* CONTEXTMENUEVENTSOURCE_H_ */
