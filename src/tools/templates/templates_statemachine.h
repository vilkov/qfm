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
#ifndef TEMPLATES_STATEMACHINE_H_
#define TEMPLATES_STATEMACHINE_H_

#include <stddef.h>
#include "templates_ns.h"
#include "loki-0.1.7/Typelist.h"


#define RULE(FromState, Event, ToState, Context, ContextMethod) \
	::Tools::Templates::Rule<Context>::Definition<FromState, Event, ToState, &Context::ContextMethod>


TEMPLATES_NS_BEGIN

template <typename Context>
struct Rule
{
	typedef void (Context::*Action)(const char *input, int size);

	template <int FromState, int Token, int ToState, Action action>
	struct Definition
	{
		typedef Context rule_context;

		enum
        {
        	FROM_STATE = FromState,
        	TOKEN = Token,
        	TO_STATE = ToState
        };

		static inline void invoke(Context *context, const char *input, int size)
		{
			(context->*action)(input, size);
		}
	};
};


struct Data
{
	void *context;
	int state;

	const char *iterator;
	const char *input;
};


template <typename Context, typename RulesTypeList>
struct ProcessEvent
{
	static inline void process(Data &data)
	{
		typedef typename RulesTypeList::Head RulesTypeList_Item;

		if (RulesTypeList_Item::FROM_STATE == data.state && RulesTypeList_Item::TOKEN == *data.iterator)
		{
			data.state = RulesTypeList_Item::TO_STATE;
			RulesTypeList_Item::invoke(static_cast<typename RulesTypeList_Item::rule_context *>(data.context), data.input, data.iterator - data.input);
			data.input = data.iterator + 1;
			return;
		}

		ProcessEvent<Context, typename RulesTypeList::Tail>::process(data);
	}
};

template <typename Context>
struct ProcessEvent<Context, ::Loki::NullType>
{
	static inline void process(Data &data)
	{}
};


template <typename RulesTypeList>
class StateMachine
{
public:
	typedef typename RulesTypeList::Head::rule_context Context;

public:
	StateMachine(Context &context)
	{
		m_data.context = &context;
		m_data.state = RulesTypeList::Head::FROM_STATE;
		m_data.iterator = NULL;
		m_data.input = NULL;
	}

	StateMachine(Context *context)
	{
		m_data.context = context;
		m_data.state = RulesTypeList::Head::FROM_STATE;
		m_data.iterator = NULL;
		m_data.input = NULL;
	}

	inline int process(const char *string)
	{
		for (m_data.input = m_data.iterator = string; *m_data.iterator; ++m_data.iterator)
			ProcessEvent<Context, RulesTypeList>::process(m_data);

		ProcessEvent<Context, RulesTypeList>::process(m_data);

		return m_data.state;
	}

protected:
	Data m_data;
};

TEMPLATES_NS_END

#endif /* TEMPLATES_STATEMACHINE_H_ */
