#ifndef IDMQUERYRESULTSFUNCTOR_H_
#define IDMQUERYRESULTSFUNCTOR_H_

#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include "../items/idmqueryresultvalueitem.h"


IDM_PLUGIN_NS_BEGIN

class Functor
{
public:
	virtual ~Functor() {}

	inline void operator()(const QModelIndex &index, QueryResultValueItem *value) const { return call(index, value); }

protected:
	virtual void call(const QModelIndex &index, QueryResultValueItem *value) const = 0;
};


/***************************************************************************************************/
template <typename T>
class Callable : public Functor
{
public:
	typedef void (T::*Method)(const QModelIndex &index, QueryResultValueItem *value);

public:
	Callable(T *object, Method method) :
		m_object(object),
		m_method(method)
	{}

protected:
	virtual void call(const QModelIndex &index, QueryResultValueItem *value) const { return (m_object->*m_method)(index, value); }

private:
	T *m_object;
	Method m_method;
};
template <typename T>
inline static Callable<T> callTo(T *object, typename Callable<T>::Method method)
{ return Callable<T>(object, method); }


/***************************************************************************************************/
template <typename T, typename Arg>
class Callable1 : public Functor
{
public:
	typedef void (T::*Method)(Arg arg, const QModelIndex &index, QueryResultValueItem *value);

public:
	Callable1(T *object, const Arg &arg, Method method) :
		m_object(object),
		m_arg(arg),
		m_method(method)
	{}

protected:
	virtual void call(const QModelIndex &index, QueryResultValueItem *value) const { return (m_object->*m_method)(m_arg, index, value); }

private:
	T *m_object;
	Arg m_arg;
	Method m_method;
};
template <typename T, typename Arg>
inline static Callable1<T, Arg> callTo(T *object, const Arg &arg, typename Callable1<T, Arg>::Method method)
{ return Callable1<T, Arg>(object, arg, method); }

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTSFUNCTOR_H_ */
