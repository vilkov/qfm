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
#ifndef EVENTSOURCEBASE_H_
#define EVENTSOURCEBASE_H_

#include <QtGui/QWidget>
#include "../eventhandler.h"
#include "../../../templates/metatemplates.h"


EVENTS_NS_BEGIN

#include "../../../arguments/define.h"
#define INITIALISERS(Values) :         \
		BaseClass(Values),             \
	    m_eventHandler(eventHandler)   \
	{                                  \
		Q_ASSERT(m_eventHandler != 0); \
	}                                  \

template <typename BaseClass>
class EventSourceBase : public BaseClass
{
public:
	EventSourceBase(EventHandler *eventHandler) :
		BaseClass(),
	    m_eventHandler(eventHandler)
	{
		Q_ASSERT(m_eventHandler != 0);
	}

    template <TEMPLATE_ARGS_1>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_1) INITIALISERS(VALUES_1)

    template <TEMPLATE_ARGS_2>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_2) INITIALISERS(VALUES_2)

    template <TEMPLATE_ARGS_3>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_3) INITIALISERS(VALUES_3)

    template <TEMPLATE_ARGS_4>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_4) INITIALISERS(VALUES_4)

    template <TEMPLATE_ARGS_5>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_5) INITIALISERS(VALUES_5)

    template <TEMPLATE_ARGS_6>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_6) INITIALISERS(VALUES_6)

    template <TEMPLATE_ARGS_7>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_7) INITIALISERS(VALUES_7)

    template <TEMPLATE_ARGS_8>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_8) INITIALISERS(VALUES_8)

    template <TEMPLATE_ARGS_9>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_9) INITIALISERS(VALUES_9)

    template <TEMPLATE_ARGS_10>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_BY_REFERENCE_10) INITIALISERS(VALUES_10)

protected:
    inline EventHandler *eventHandler() const { return m_eventHandler; }

private:
    EventHandler *m_eventHandler;
};

#undef INITIALISERS
#include "../../../arguments/undefine.h"

EVENTS_NS_END

#endif /* EVENTSOURCEBASE_H_ */
