#ifndef FUNCTORS_H_
#define FUNCTORS_H_

#include "templates_ns.h"
#include "../arguments/define.h"


TEMPLATES_NS_BEGIN

class Functor
{
public:
	virtual ~Functor() {}

	inline void operator()() const { call(); }

protected:
	virtual void call() const = 0;
};


template <TEMPLATE_ARGS_1>
class Functor1
{
public:
	virtual ~Functor1() {}

	inline void operator()(ARGUMENTS_BY_VALUE_1) const { call(VALUES_1); }

protected:
	virtual void call(ARGUMENTS_BY_VALUE_1) const = 0;
};


template <TEMPLATE_ARGS_2>
class Functor2
{
public:
	virtual ~Functor2() {}

	inline void operator()(ARGUMENTS_BY_VALUE_2) const { call(VALUES_2); }

protected:
	virtual void call(ARGUMENTS_BY_VALUE_2) const = 0;
};


template <TEMPLATE_ARGS_3>
class Functor3
{
public:
	virtual ~Functor3() {}

	inline void operator()(ARGUMENTS_BY_VALUE_3) const { call(VALUES_3); }

protected:
	virtual void call(ARGUMENTS_BY_VALUE_3) const = 0;
};


/***************************************************************************************************/
template <typename T>
class Callable : public Functor
{
public:
	typedef void (T::*Method)();

public:
	Callable(T *object, Method method) :
		m_object(object),
		m_method(method)
	{}

protected:
	virtual void call() const { (m_object->*m_method)(); }

private:
	T *m_object;
	Method m_method;
};
template <typename T>
inline Callable<T> callTo(T *object, typename Callable<T>::Method method)
{ return Callable<T>(object, method); }


/***************************************************************************************************/
template <typename T, typename Arg1>
class Callable1 : public Functor
{
public:
	typedef void (T::*Method)(Arg1);

public:
	Callable1(T *object, Method method, Arg1 arg1) :
		m_object(object),
		m_method(method),
		m_arg1(arg1)
	{}

protected:
	virtual void call() const { (m_object->*m_method)(m_arg1); }

private:
	T *m_object;
	Method m_method;
	Arg1 m_arg1;
};
template <typename T, typename Arg1>
inline Callable1<T, Arg1> callTo(T *object, typename Callable1<T, Arg1>::Method method, Arg1 arg1)
{ return Callable1<T, Arg1>(object, method, arg1); }


/***************************************************************************************************/
template <typename T, typename Arg1, typename Arg2>
class Callable2 : public Functor
{
public:
	typedef void (T::*Method)(Arg1, Arg2);

public:
	Callable2(T *object, Method method, Arg1 arg1, Arg2 arg2) :
		m_object(object),
		m_method(method),
		m_arg1(arg1),
		m_arg2(arg2)
	{}

protected:
	virtual void call() const { (m_object->*m_method)(m_arg1, m_arg2); }

private:
	T *m_object;
	Method m_method;
	Arg1 m_arg1;
	Arg2 m_arg2;
};
template <typename T, typename Arg1, typename Arg2>
inline Callable2<T, Arg1, Arg2> callTo(T *object, typename Callable2<T, Arg1, Arg2>::Method method, Arg1 arg1, Arg2 arg2)
{ return Callable2<T, Arg1, Arg2>(object, method, arg1, arg2); }

TEMPLATES_NS_END

#include "../arguments/undefine.h"

#endif /* FUNCTORS_H_ */
