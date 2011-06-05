#ifndef FILESYSTEMFOLDERNODEFUNCTORS_H_
#define FILESYSTEMFOLDERNODEFUNCTORS_H_

#include "../items/filesystemfoldernodeitem.h"


/**
 *  Closed namespace "FolderNodeFunctors"
 */
struct FolderNodeFunctors
{
	class Functor
	{
	public:
		typedef FileSystem::FolderNodeItem Entry;

	public:
		virtual ~Functor() {}

		inline void operator()(Entry *entry) const { call(entry); }

	protected:
		virtual void call(Entry *entry) const = 0;
	};


	/***************************************************************************************************/
	template <typename T>
	class Callable : public Functor
	{
	public:
		typedef void (T::*Method)(Entry *);

	public:
		Callable(T *object, Method method) :
			m_object(object),
			m_method(method)
		{}

	protected:
		virtual void call(Entry *entry) const { (m_object->*m_method)(entry); }

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
		typedef void (T::*Method)(Entry *, Arg1);

	public:
		Callable1(T *object, Method method, Arg1 arg1) :
			m_object(object),
			m_method(method),
			m_arg1(arg1)
		{}

	protected:
		virtual void call(Entry *entry) const { (m_object->*m_method)(entry, m_arg1); }

	private:
		T *m_object;
		Method m_method;
		Arg1 m_arg1;
	};
	template <typename T, typename Arg1>
	inline static Callable1<T, Arg1> callTo(T *object, typename Callable1<T, Arg1>::Method method, Arg1 arg1)
	{ return Callable1<T, Arg1>(object, method, arg1); }
};

#endif /* FILESYSTEMFOLDERNODEFUNCTORS_H_ */
