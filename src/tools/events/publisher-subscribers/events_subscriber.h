#ifndef EVENTS_SUBSCRIBER_H_
#define EVENTS_SUBSCRIBER_H_

#include "../events_ns.h"
#include "../../arguments/define.h"


#define DECLARE_EVENT_SUBSCRIBER(Event)                                   \
	template <typename T>                                                 \
	class SubscribeTo##Event : public ::Tools::Events::Subscribe<T>       \
	{                                                                     \
		typedef ::Tools::Events::Subscribe<T> BaseClass;                  \
		typedef typename BaseClass::Method    Method;                     \
                                                                          \
	public:                                                               \
		SubscribeTo##Event(T *object, Method method) :                    \
			BaseClass::Subscribe(object, method, static_cast<int>(Event)) \
		{}                                                                \
	}

#define DECLARE_EVENT_SUBSCRIBER_1(Event, Arg1)                            \
	template <typename T>                                                  \
	class SubscribeTo##Event : public ::Tools::Events::Subscribe1<T, Arg1> \
	{                                                                      \
		typedef ::Tools::Events::Subscribe1<T, Arg1> BaseClass;            \
		typedef typename BaseClass::Method           Method;               \
                                                                           \
	public:                                                                \
		SubscribeTo##Event(T *object, Method method) :                     \
			BaseClass(object, method, static_cast<int>(Event))             \
		{}                                                                 \
	};                                                                     \
	struct Event##Arguments                                                \
	{                                                                      \
		typedef Arg1 Argument1;                                            \
	}

#define DECLARE_EVENT_SUBSCRIBER_2(Event, Arg1, Arg2)                            \
	template <typename T>                                                        \
	class SubscribeTo##Event : public ::Tools::Events::Subscribe2<T, Arg1, Arg2> \
	{                                                                            \
		typedef ::Tools::Events::Subscribe2<T, Arg1, Arg2> BaseClass;            \
		typedef typename BaseClass::Method                 Method;               \
                                                                                 \
	public:                                                                      \
		SubscribeTo##Event(T *object, Method method) :                           \
			BaseClass(object, method, static_cast<int>(Event))                   \
		{}                                                                       \
	};                                                                           \
	struct Event##Arguments                                                      \
	{                                                                            \
		typedef Arg1 Argument1;                                                  \
		typedef Arg2 Argument2;                                                  \
	}

#define DECLARE_EVENT_SUBSCRIBER_3(Event, Arg1, Arg2, Arg3)                            \
	template <typename T>                                                              \
	class SubscribeTo##Event : public ::Tools::Events::Subscribe3<T, Arg1, Arg2, Arg3> \
	{                                                                                  \
		typedef ::Tools::Events::Subscribe3<T, Arg1, Arg2, Arg3> BaseClass;            \
		typedef typename BaseClass::Method                       Method;               \
                                                                                       \
	public:                                                                            \
		SubscribeTo##Event(T *object, Method method) :                                 \
			BaseClass(object, method, static_cast<int>(Event))                         \
		{}                                                                             \
	};                                                                                 \
	struct Event##Arguments                                                            \
	{                                                                                  \
		typedef Arg1 Argument1;                                                        \
		typedef Arg2 Argument2;                                                        \
		typedef Arg3 Argument3;                                                        \
	}


EVENTS_NS_BEGIN

struct Arguments
{};

class Subscriber
{
public:
	Subscriber(int event) :
		m_event(event)
	{}
	virtual ~Subscriber();

private:
	friend class Publisher;
	virtual void receive(const Arguments &arguments) = 0;

private:
	int event() const { return m_event; }

private:
	int m_event;
};


/**
 * Without arguments
 */
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

private:
	virtual void receive(const Arguments &arguments)
	{
		m_object->*m_method();
	}

private:
	T *m_object;
	Method m_method;
};


/**
 * One argument
 */
template <TEMPLATE_ARGS_1>
struct Arguments1 : public Arguments
{
	Arguments1(ARGUMENTS_BY_VALUE_1) :
		INITIALIZE_ARGUMENTS_1
	{}

	DECLARE_ARGUMENTS_1;
};

template <typename T, TEMPLATE_ARGS_1>
class Subscribe1 : public Subscriber
{
public:
	typedef void (T::*Method)(ARGUMENTS_BY_VALUE_1);

public:
	Subscribe1(T *object, Method method, int event) :
		Subscriber(event),
		m_object(object),
		m_method(method)
	{
		Q_ASSERT(m_object && m_method);
	}

private:
	virtual void receive(const Arguments &arguments)
	{
		const Arguments1<ARGUMENTS_1> &args = reinterpret_cast<const Arguments1<ARGUMENTS_1> &>(arguments);
		(m_object->*m_method)(args.value1);
	}

private:
	T *m_object;
	Method m_method;
};


/**
 * Two arguments
 */
template <TEMPLATE_ARGS_2>
struct Arguments2 : public Arguments
{
	Arguments2(ARGUMENTS_BY_VALUE_2) :
		INITIALIZE_ARGUMENTS_2
	{}

	DECLARE_ARGUMENTS_2;
};

template <typename T, TEMPLATE_ARGS_2>
class Subscribe2 : public Subscriber
{
public:
	typedef void (T::*Method)(ARGUMENTS_BY_VALUE_2);

public:
	Subscribe2(T *object, Method method, int event) :
		Subscriber(event),
		m_object(object),
		m_method(method)
	{
		Q_ASSERT(m_object && m_method);
	}

private:
	virtual void receive(const Arguments &arguments)
	{
		const Arguments2<ARGUMENTS_2> &args = reinterpret_cast<const Arguments2<ARGUMENTS_2> &>(arguments);
		(m_object->*m_method)(args.value1, args.value2);
	}

private:
	T *m_object;
	Method m_method;
};


/**
 * Three arguments
 */
template <TEMPLATE_ARGS_3>
struct Arguments3 : public Arguments
{
	Arguments3(ARGUMENTS_BY_VALUE_3) :
		INITIALIZE_ARGUMENTS_3
	{}

	DECLARE_ARGUMENTS_3;
};

template <typename T, TEMPLATE_ARGS_3>
class Subscribe3 : public Subscriber
{
public:
	typedef void (T::*Method)(ARGUMENTS_BY_VALUE_3);

public:
	Subscribe3(T *object, Method method, int event) :
		Subscriber(event),
		m_object(object),
		m_method(method)
	{
		Q_ASSERT(m_object && m_method);
	}

private:
	virtual void receive(const Arguments &arguments)
	{
		const Arguments3<ARGUMENTS_3> &args = reinterpret_cast<const Arguments3<ARGUMENTS_3> &>(arguments);
		(m_object->*m_method)(args.value1, args.value2, args.value3);
	}

private:
	T *m_object;
	Method m_method;
};

EVENTS_NS_END


#include "../../arguments/undefine.h"

#endif /* EVENTS_SUBSCRIBER_H_ */
