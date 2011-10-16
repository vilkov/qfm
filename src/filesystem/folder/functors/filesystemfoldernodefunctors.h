#ifndef FILESYSTEMFOLDERNODEFUNCTORS_H_
#define FILESYSTEMFOLDERNODEFUNCTORS_H_

#include <QtCore/QList>
#include "../base/items/filesystembaseitem.h"
#include "../containers/filesystemfoldernodevalues.h"


FILE_SYSTEM_NS_BEGIN

/**
 *  Closed namespace "Functors"
 */
struct Functors
{
	class Functor
	{
	public:
		virtual ~Functor() {}

		inline void operator()(Values::size_type index, FileSystemBaseItem *entry) { call(index, entry); }

	protected:
		virtual void call(Values::size_type index, FileSystemBaseItem *entry) = 0;
	};


	/***************************************************************************************************/
	template <typename T>
	class Callable : public Functor
	{
	public:
		typedef void (T::*Method)(Values::size_type index, FileSystemBaseItem *entry);

	public:
		Callable(T *object, Method method) :
			m_object(object),
			m_method(method)
		{}

	protected:
		virtual void call(Values::size_type index, FileSystemBaseItem *entry) { (m_object->*m_method)(index, entry); }

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
		typedef void (T::*Method)(Values::size_type index, FileSystemBaseItem *entry, Arg1);

	public:
		Callable1(T *object, Method method, Arg1 arg1) :
			m_object(object),
			m_method(method),
			m_arg1(arg1)
		{}

	protected:
		virtual void call(Values::size_type index, FileSystemBaseItem *entry) { (m_object->*m_method)(index, entry, m_arg1); }

	private:
		T *m_object;
		Method m_method;
		Arg1 m_arg1;
	};
	template <typename T, typename Arg1>
	inline static Callable1<T, Arg1> callTo(T *object, typename Callable1<T, Arg1>::Method method, Arg1 arg1)
	{ return Callable1<T, Arg1>(object, method, arg1); }
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEFUNCTORS_H_ */
