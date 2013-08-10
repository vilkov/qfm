/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef FOCUSEVENTSOURCE_H_
#define FOCUSEVENTSOURCE_H_

#include "eventsourcebase.h"


EVENTS_NS_BEGIN

#include "../../../arguments/define.h"

template <typename BaseClass = EventSourceBase<QWidget> >
class FocusOutEventSource : public BaseClass
{
public:
	FocusOutEventSource(EventHandler *eventHandler) :
		BaseClass(eventHandler)
	{}

    template <TEMPLATE_ARGS_1>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_1) :
		BaseClass(eventHandler, VALUES_1)
	{}

    template <TEMPLATE_ARGS_2>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_2) :
		BaseClass(eventHandler, VALUES_2)
	{}

    template <TEMPLATE_ARGS_3>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_3) :
		BaseClass(eventHandler, VALUES_3)
	{}

    template <TEMPLATE_ARGS_4>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_4) :
		BaseClass(eventHandler, VALUES_4)
	{}

    template <TEMPLATE_ARGS_5>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_5) :
		BaseClass(eventHandler, VALUES_5)
	{}

    template <TEMPLATE_ARGS_6>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_6) :
		BaseClass(eventHandler, VALUES_6)
	{}

    template <TEMPLATE_ARGS_7>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_7) :
		BaseClass(eventHandler, VALUES_7)
	{}

    template <TEMPLATE_ARGS_8>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_8) :
		BaseClass(eventHandler, VALUES_8)
	{}

    template <TEMPLATE_ARGS_9>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_9) :
		BaseClass(eventHandler, VALUES_9)
	{}

    template <TEMPLATE_ARGS_10>
    FocusOutEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_10) :
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
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_1) :
		BaseClass(eventHandler, VALUES_1)
	{}

    template <TEMPLATE_ARGS_2>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_2) :
		BaseClass(eventHandler, VALUES_2)
	{}

    template <TEMPLATE_ARGS_3>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_3) :
		BaseClass(eventHandler, VALUES_3)
	{}

    template <TEMPLATE_ARGS_4>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_4) :
		BaseClass(eventHandler, VALUES_4)
	{}

    template <TEMPLATE_ARGS_5>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_5) :
		BaseClass(eventHandler, VALUES_5)
	{}

    template <TEMPLATE_ARGS_6>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_6) :
		BaseClass(eventHandler, VALUES_6)
	{}

    template <TEMPLATE_ARGS_7>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_7) :
		BaseClass(eventHandler, VALUES_7)
	{}

    template <TEMPLATE_ARGS_8>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_8) :
		BaseClass(eventHandler, VALUES_8)
	{}

    template <TEMPLATE_ARGS_9>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_9) :
		BaseClass(eventHandler, VALUES_9)
	{}

    template <TEMPLATE_ARGS_10>
    FocusInEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_10) :
		BaseClass(eventHandler, VALUES_10)
	{}

protected:
	virtual void focusInEvent(QFocusEvent *event)
    {
    	if (!BaseClass::eventHandler()->focusInEvent(event))
    		BaseClass::focusInEvent(event);
    }
};

#include "../../../arguments/undefine.h"

EVENTS_NS_END

#endif /* FOCUSEVENTSOURCE_H_ */
