#ifndef EVENTS_PUBLISHER_H_
#define EVENTS_PUBLISHER_H_

#include <QtCore/QMap>
#include <QtCore/QLinkedList>
#include "events_subscriber.h"
#include "../../arguments/define.h"


EVENTS_NS_BEGIN

class Publisher
{
private:
	typedef QLinkedList<Subscriber *> List;
	typedef QMap<int, List>           Container;

public:
	Publisher()
	{}
	virtual ~Publisher();

	void subscribe(Subscriber *subscriber)
	{
		Q_ASSERT(!m_subscribers[subscriber->event()].contains(subscriber));
		m_subscribers[subscriber->event()].push_back(subscriber);
	}

	void unsubscribe(Subscriber *subscriber)
	{
		Q_ASSERT(!m_subscribers.value(subscriber->event()).isEmpty());
		List &list = m_subscribers[subscriber->event()];

		for (List::iterator i = list.begin(), end = list.end(); i != end; ++i)
			if ((*i) == subscriber)
			{
				list.erase(i);
				break;
			}
	}

protected:
	inline void publish(int event)
	{
		Arguments arguments;
		publish(event, arguments);
	}

	template <TEMPLATE_ARGS_1>
	inline void publish(int event, ARGUMENTS_BY_VALUE_1)
	{
		Arguments1<ARGUMENTS_1> arguments(VALUES_1);
		publish(event, static_cast<Arguments &>(arguments));
	}

	template <TEMPLATE_ARGS_2>
	inline void publish(int event, ARGUMENTS_BY_VALUE_2)
	{
		Arguments2<ARGUMENTS_2> arguments(VALUES_2);
		publish(event, static_cast<Arguments &>(arguments));
	}

	template <TEMPLATE_ARGS_3>
	inline void publish(int event, ARGUMENTS_BY_VALUE_3)
	{
		Arguments3<ARGUMENTS_3> arguments(VALUES_3);
		publish(event, static_cast<Arguments &>(arguments));
	}

private:
	inline void publish(int event, Arguments &arguments)
	{
		List &list = m_subscribers[event];

		for (List::iterator i = list.begin(), end = list.end(); i != end; ++i)
			(*i)->receive(arguments);
	}

private:
	Container m_subscribers;
};

EVENTS_NS_END


#include "../../arguments/undefine.h"

#endif /* EVENTS_PUBLISHER_H_ */
