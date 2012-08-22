#ifndef EVENTS_SUBSCRIBER_H_
#define EVENTS_SUBSCRIBER_H_

#include "../events_ns.h"


EVENTS_NS_BEGIN

class Subscriber
{
public:
	Subscriber(int event) :
		m_event(event)
	{}
	virtual ~Subscriber();

protected:
	friend class Publisher;
	virtual void receive() = 0;

private:
	int event() const { return m_event; }

private:
	int m_event;
};


template <typename T>
class Subscribe : public Subscriber
{
public:
	typedef void (T::*Method)();

public:
	Subscribe(T *object, Method method, int event) :
		Subscriber(event),
		m_object(object),
		m_method(method)
	{
		Q_ASSERT(m_object && m_method);
	}

protected:
	virtual void receive()
	{
		m_object->*m_method();
	}

private:
	T *m_object;
	Method m_method;
};

EVENTS_NS_END

#endif /* EVENTS_SUBSCRIBER_H_ */
