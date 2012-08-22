#ifndef EVENTS_PUBLISHER_H_
#define EVENTS_PUBLISHER_H_

#include <QtCore/QMap>
#include <QtCore/QLinkedList>
#include "events_subscriber.h"


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
	void publish(int event)
	{
		List &list = m_subscribers[event];

		for (List::iterator i = list.begin(), end = list.end(); i != end; ++i)
			(*i)->receive();
	}

private:
	Container m_subscribers;
};

EVENTS_NS_END

#endif /* EVENTS_PUBLISHER_H_ */
