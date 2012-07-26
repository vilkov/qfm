#ifndef METATEMPLATES_H_
#define METATEMPLATES_H_

#include "templates_ns.h"


TEMPLATES_NS_BEGIN

class null_type {};
struct empty_type {};

template <int T>
struct int_to_type
{
	enum { value = T };
};

template <typename T>
struct type_to_type
{
	typedef T type;
};


/***************************************************************************************************/
template <typename T>
struct is_integral_type
{
	enum { value = 0 };
};

template <>
struct is_integral_type<char>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<unsigned char>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<short>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<unsigned short>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<int>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<unsigned int>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<long>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<unsigned long>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<long long>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<unsigned long long>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<double>
{
	enum { value = 1 };
};

template <>
struct is_integral_type<float>
{
	enum { value = 1 };
};


/***************************************************************************************************/
template <bool B>
struct bool_value
{
	enum { value = B };
};


/***************************************************************************************************/
template <typename T, typename U>
class conversion
{
	typedef char one_t;
	typedef struct { char array[2]; } two_t;
	static one_t test(U);
	static two_t test(...);
	static T makeT();

public:
	enum { exists = sizeof(test(makeT())) == sizeof(one_t) };
	enum { is_same_types = 0 };
};

template <typename T>
class conversion<T, T>
{
public:
	enum { exists = 1 };
	enum { is_same_types = 1 };
};


template <typename T>
class has_value_type
{
	typedef char one_t;
	typedef struct { char array[2]; } two_t;
	template <typename U> static one_t check_function(...);
	template <typename U> static two_t check_function(typename U::value_type const volatile *);

public:
	enum { value = sizeof(check_function<T>(0)) == sizeof(two_t) };
};

template <typename T>
class has_size_type
{
	typedef char one_t;
	typedef struct { char array[2]; } two_t;
	template <typename U> static one_t check_function(...);
	template <typename U> static two_t check_function(typename U::size_type const volatile *);

public:
	enum { value = sizeof(check_function<T>(0)) == sizeof(two_t) };
};


/***************************************************************************************************/
template <typename T>
struct remove_qualifiers
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<T*>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<T&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<T*&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<T* const &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<T* volatile &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<T* const volatile &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const T>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const T*>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const T&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const T*&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const T* const &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const T* volatile &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const T* const volatile &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T*>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T*&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T* const &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T* volatile &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<volatile T* const volatile &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T*>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T*&>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T* const &>
{
	typedef T type;
};
template <typename T>
struct remove_qualifiers<const volatile T* const volatile &>
{
	typedef T type;
};


/***************************************************************************************************/
template <typename Derived, typename Base>
struct must_have_base
{
	typedef typename remove_qualifiers<Derived>::type first_t;
	typedef typename remove_qualifiers<Base>::type second_t;

public:
	~must_have_base() throw()
	{
		void(*p)(first_t*, second_t*) = constraints;
		(void)p;
	}

private:
	static void constraints(first_t *pd, second_t *pb)
	{
		pb = pd;
		(void)pb;
	}
};


/***************************************************************************************************/
template <typename T1, typename T2, bool B>
struct select_first_type_if
{
	typedef T2 type;
};

template <typename T1, typename T2>
struct select_first_type_if<T1, T2, true>
{
	typedef T1 type;
};


/***************************************************************************************************/
template <typename T, typename U>
struct type_list
{
	typedef T head;
	typedef U tail;
};

template <typename List, unsigned int Index = 0> struct type_at;

template <typename Head>
struct type_at<Head, 0>
{
	typedef Head type;
};

template <typename Head, typename Tail>
struct type_at<type_list<Head, Tail>, 0>
{
	typedef Head type;
};

template <typename Head, typename Tail, unsigned int I>
struct type_at<type_list<Head, Tail>, I>
{
	typedef typename type_at<Tail, I-1>::type type;
};

template <typename List> struct length_of_type_list;

template <>
struct length_of_type_list<null_type>
{
	enum { value = 0 };
};

template <typename T, typename U>
struct length_of_type_list<type_list<T, U> >
{
	enum { value = 1 + length_of_type_list<U>::value };
};


#define TYPELlST_1(T1) Tools::Templates::type_list<T1, Tools::Templates::null_type>
#define TYPELlST_2(T1, T2) Tools::Templates::type_list<T1, TYPELlST_1(T2) >
#define TYPELlST_3(T1, T2, T3) Tools::Templates::type_list<T1, TYPELlST_2(T2, T3) >
#define TYPELlST_4(T1, T2, T3, T4) Tools::Templates::type_list<T1, TYPELlST_3(T2, T3, T4) >
#define TYPELlST_5(T1, T2, T3, T4, T5) Tools::Templates::type_list<T1, TYPELlST_4(T2, T3, T4, T5) >


/***************************************************************************************************/
namespace Adaptors
{
	template <typename T, typename ReferenceToContainer = bool_value<true> >
	struct BaseContainer
	{
	public:
		typedef typename remove_qualifiers<typename T::value_type>::type                       value_type;
		typedef typename T::size_type                                                          size_type;
		typedef typename select_first_type_if<const T &, T, ReferenceToContainer::value>::type container_type;

	public:
		BaseContainer(const T &container) :
			m_container(container)
		{}

		inline bool isEmpty() const { return m_container.isEmpty(); }
		inline size_type size() const { return m_container.size(); }

	protected:
		container_type m_container;

	private:
		BaseContainer(const BaseContainer<T> &other);
		BaseContainer<T> &operator=(const BaseContainer<T> &other);
	};

	/***************************************************************************************************/
	template <typename T, typename ReferenceToContainer = bool_value<false> >
	struct StrictBaseContainer : public BaseContainer<T, ReferenceToContainer>
	{
	public:
		typedef BaseContainer<T, ReferenceToContainer> parent_class;
		typedef typename parent_class::value_type      value_type;
		typedef typename parent_class::size_type       size_type;

	public:
		StrictBaseContainer(const T &container) :
			parent_class(container)
		{}

		inline value_type at(size_type i) const;
		inline value_type operator[](size_type i) const;

	private:
		StrictBaseContainer(const StrictBaseContainer<T> &other);
		StrictBaseContainer<T> &operator=(const StrictBaseContainer<T> &other);
	};

	/***************************************************************************************************/
	template <typename T, typename ReferenceToContainer = bool_value<true> >
	struct SoftBaseContainer : public BaseContainer<T, ReferenceToContainer>
	{
	public:
		typedef BaseContainer<T, ReferenceToContainer> parent_class;
		typedef typename parent_class::value_type      value_type;
		typedef typename parent_class::size_type       size_type;

	public:
		SoftBaseContainer(const T &container) :
			parent_class(container)
		{}

		inline const value_type &at(size_type i) const;
		inline const value_type &operator[](size_type i) const;

	private:
		SoftBaseContainer(const SoftBaseContainer<T> &other);
		SoftBaseContainer<T> &operator=(const SoftBaseContainer<T> &other);
	};


	/***************************************************************************************************/
	template <typename T, typename ReferenceToContainer = bool_value<true> >
	struct PointersContainer : public SoftBaseContainer<T, ReferenceToContainer>
	{
	public:
		typedef SoftBaseContainer<T,ReferenceToContainer> parent_class;
		typedef typename parent_class::value_type         value_type;
		typedef typename parent_class::size_type          size_type;

	public:
		PointersContainer(const T &container) :
			parent_class(container)
		{}

		inline const value_type &at(size_type i) const { return *parent_class::m_container.at(i); }
		inline const value_type &operator[](size_type i) const { return *(parent_class::m_container[i]); }

	private:
		PointersContainer(const PointersContainer<T> &other);
		PointersContainer<T> &operator=(const PointersContainer<T> &other);
	};
}


/***************************************************************************************************/
namespace Utils
{
	template <typename T>
	inline T takeFrom(T &value)
	{
		T res(value);
		value = T();
		return res;
	}

	template <typename T>
	inline void nullify(T *&value)
	{
		delete value;
		value = 0;
	}
}

TEMPLATES_NS_END

#endif /* METATEMPLATES_H_ */
