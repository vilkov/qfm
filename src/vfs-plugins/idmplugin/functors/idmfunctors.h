#ifndef IDMFUNCTORS_H_
#define IDMFUNCTORS_H_

#include <QtCore/QModelIndex>
#include "../model/items/idmitem.h"
#include "../../../filesystem/filesystem_ns.h"


IDM_PLUGIN_NS_BEGIN

/**
 *  Closed namespace "IdmFunctors"
 */
struct IdmFunctors
{
	class Functor
	{
	public:
		virtual ~Functor() {}

		inline bool operator()(const QModelIndex &index, IdmItem *item) const { return call(index, item); }

	protected:
		virtual bool call(const QModelIndex &index, IdmItem *item) const = 0;
	};


	/***************************************************************************************************/
	template <typename T>
	class Callable : public Functor
	{
	public:
		typedef bool (T::*Method)(const QModelIndex &index, IdmItem *item);

	public:
		Callable(T *object, Method method) :
			m_object(object),
			m_method(method)
		{}

	protected:
		virtual bool call(const QModelIndex &index, IdmItem *item) const { return (m_object->*m_method)(index, item); }

	private:
		T *m_object;
		Method m_method;
	};
	template <typename T>
	inline static Callable<T> callTo(T *object, typename Callable<T>::Method method)
	{ return Callable<T>(object, method); }


	/***************************************************************************************************/
	template <typename T, typename Arg1>
	class Callable1 : public Functor
	{
	public:
		typedef bool (T::*Method)(const QModelIndex &index, IdmItem *item, Arg1);

	public:
		Callable1(T *object, Method method, Arg1 arg1) :
			m_object(object),
			m_method(method),
			m_arg1(arg1)
		{}

	protected:
		virtual bool call(const QModelIndex &index, IdmItem *item) const { return (m_object->*m_method)(index, item, m_arg1); }

	private:
		T *m_object;
		Method m_method;
		Arg1 m_arg1;
	};
	template <typename T, typename Arg1>
	inline static Callable1<T, Arg1> callTo(T *object, typename Callable1<T, Arg1>::Method method, Arg1 arg1)
	{ return Callable1<T, Arg1>(object, method, arg1); }
};

IDM_PLUGIN_NS_END

#endif /* IDMFUNCTORS_H_ */
