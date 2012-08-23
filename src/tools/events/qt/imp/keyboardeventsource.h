#ifndef KEYBOARDEVENTSOURCE_H_
#define KEYBOARDEVENTSOURCE_H_

#include "eventsourcebase.h"


EVENTS_NS_BEGIN

#include "../../../arguments/define.h"

template <typename BaseClass = EventSourceBase<QWidget> >
class KeyboardEventSource : public BaseClass
{
public:
	KeyboardEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <TEMPLATE_ARGS_1>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_1) :
		BaseClass(eventHandler, VALUES_1)
	{}

    template <TEMPLATE_ARGS_2>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_2) :
		BaseClass(eventHandler, VALUES_2)
	{}

    template <TEMPLATE_ARGS_3>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_3) :
		BaseClass(eventHandler, VALUES_3)
	{}

    template <TEMPLATE_ARGS_4>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_4) :
		BaseClass(eventHandler, VALUES_4)
	{}

    template <TEMPLATE_ARGS_5>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_5) :
		BaseClass(eventHandler, VALUES_5)
	{}

    template <TEMPLATE_ARGS_6>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_6) :
		BaseClass(eventHandler, VALUES_6)
	{}

    template <TEMPLATE_ARGS_7>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_7) :
		BaseClass(eventHandler, VALUES_7)
	{}

    template <TEMPLATE_ARGS_8>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_8) :
		BaseClass(eventHandler, VALUES_8)
	{}

    template <TEMPLATE_ARGS_9>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_9) :
		BaseClass(eventHandler, VALUES_9)
	{}

    template <TEMPLATE_ARGS_10>
    KeyboardEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_10) :
		BaseClass(eventHandler, VALUES_10)
	{}

protected:
    virtual void keyPressEvent(QKeyEvent *event)
    {
    	if (!BaseClass::eventHandler()->keyboardEvent(event))
    		BaseClass::keyPressEvent(event);
    }
};

#include "../../../arguments/undefine.h"

EVENTS_NS_END

#endif /* KEYBOARDEVENTSOURCE_H_ */
