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
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_1) :
		BaseClass(eventHandler, VALUES_1)
	{}

    template <TEMPLATE_ARGS_2>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_2) :
		BaseClass(eventHandler, VALUES_2)
	{}

    template <TEMPLATE_ARGS_3>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_3) :
		BaseClass(eventHandler, VALUES_3)
	{}

    template <TEMPLATE_ARGS_4>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_4) :
		BaseClass(eventHandler, VALUES_4)
	{}

    template <TEMPLATE_ARGS_5>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_5) :
		BaseClass(eventHandler, VALUES_5)
	{}

    template <TEMPLATE_ARGS_6>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_6) :
		BaseClass(eventHandler, VALUES_6)
	{}

    template <TEMPLATE_ARGS_7>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_7) :
		BaseClass(eventHandler, VALUES_7)
	{}

    template <TEMPLATE_ARGS_8>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_8) :
		BaseClass(eventHandler, VALUES_8)
	{}

    template <TEMPLATE_ARGS_9>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_9) :
		BaseClass(eventHandler, VALUES_9)
	{}

    template <TEMPLATE_ARGS_10>
    ContextMenuEventSource(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_10) :
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
