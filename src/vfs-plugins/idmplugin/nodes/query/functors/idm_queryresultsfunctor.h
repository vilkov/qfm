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
#ifndef IDM_QUERYRESULTSFUNCTOR_H_
#define IDM_QUERYRESULTSFUNCTOR_H_

#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include "../items/idm_queryresultvalueitem.h"


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

#endif /* IDM_QUERYRESULTSFUNCTOR_H_ */
