#ifndef FOCUSEVENTSOURCE_H_
#define FOCUSEVENTSOURCE_H_

#include "eventsourcebase.h"


#include "../../arguments/define.h"

template <typename BaseClass = EventSourceBase<QWidget> >
class FocusOutEventSource : public BaseClass
{
public:
	FocusOutEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <TEMPLATE_ARGS_1>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_1) :
		BaseClass(eventHandler, VALUES_1)
	{}

    template <TEMPLATE_ARGS_2>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_2) :
		BaseClass(eventHandler, VALUES_2)
	{}

    template <TEMPLATE_ARGS_3>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_3) :
		BaseClass(eventHandler, VALUES_3)
	{}

    template <TEMPLATE_ARGS_4>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_4) :
		BaseClass(eventHandler, VALUES_4)
	{}

    template <TEMPLATE_ARGS_5>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_5) :
		BaseClass(eventHandler, VALUES_5)
	{}

    template <TEMPLATE_ARGS_6>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_6) :
		BaseClass(eventHandler, VALUES_6)
	{}

    template <TEMPLATE_ARGS_7>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_7) :
		BaseClass(eventHandler, VALUES_7)
	{}

    template <TEMPLATE_ARGS_8>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_8) :
		BaseClass(eventHandler, VALUES_8)
	{}

    template <TEMPLATE_ARGS_9>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_9) :
		BaseClass(eventHandler, VALUES_9)
	{}

    template <TEMPLATE_ARGS_10>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_10) :
		BaseClass(eventHandler, VALUES_10)
	{}

protected:
    virtual void focusOutEvent(QFocusEvent *event)
    {
    	if (!BaseClass::eventHandler()->focusOutEvent(event))
    		BaseClass::focusOutEvent(event);
    }
};


template <typename BaseClass = EventSourceBase<QWidget> >
class FocusInEventSource : public BaseClass
{
public:
	FocusInEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <TEMPLATE_ARGS_1>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_1) :
		BaseClass(eventHandler, VALUES_1)
	{}

    template <TEMPLATE_ARGS_2>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_2) :
		BaseClass(eventHandler, VALUES_2)
	{}

    template <TEMPLATE_ARGS_3>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_3) :
		BaseClass(eventHandler, VALUES_3)
	{}

    template <TEMPLATE_ARGS_4>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_4) :
		BaseClass(eventHandler, VALUES_4)
	{}

    template <TEMPLATE_ARGS_5>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_5) :
		BaseClass(eventHandler, VALUES_5)
	{}

    template <TEMPLATE_ARGS_6>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_6) :
		BaseClass(eventHandler, VALUES_6)
	{}

    template <TEMPLATE_ARGS_7>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_7) :
		BaseClass(eventHandler, VALUES_7)
	{}

    template <TEMPLATE_ARGS_8>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_8) :
		BaseClass(eventHandler, VALUES_8)
	{}

    template <TEMPLATE_ARGS_9>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_9) :
		BaseClass(eventHandler, VALUES_9)
	{}

    template <TEMPLATE_ARGS_10>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_10) :
		BaseClass(eventHandler, VALUES_10)
	{}

protected:
	virtual void focusInEvent(QFocusEvent *event)
    {
    	if (!BaseClass::eventHandler()->focusInEvent(event))
    		BaseClass::focusInEvent(event);
    }
};

#include "../../arguments/undefine.h"

#endif /* FOCUSEVENTSOURCE_H_ */
