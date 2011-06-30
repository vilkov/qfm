#ifndef MEMORYMANAGERBASE_H_
#define MEMORYMANAGERBASE_H_

#include "memory_ns.h"
#include "../metatemplates.h"

MEMORY_NS_BEGIN

#define TEMPLATE_ARG(n) typename Arg##n
#define TEMPLATE_ARGS_1  TEMPLATE_ARG(1)
#define TEMPLATE_ARGS_2  TEMPLATE_ARGS_1, TEMPLATE_ARG(2)
#define TEMPLATE_ARGS_3  TEMPLATE_ARGS_2, TEMPLATE_ARG(3)
#define TEMPLATE_ARGS_4  TEMPLATE_ARGS_3, TEMPLATE_ARG(4)
#define TEMPLATE_ARGS_5  TEMPLATE_ARGS_4, TEMPLATE_ARG(5)
#define TEMPLATE_ARGS_6  TEMPLATE_ARGS_5, TEMPLATE_ARG(6)
#define TEMPLATE_ARGS_7  TEMPLATE_ARGS_6, TEMPLATE_ARG(7)
#define TEMPLATE_ARGS_8  TEMPLATE_ARGS_7, TEMPLATE_ARG(8)
#define TEMPLATE_ARGS_9  TEMPLATE_ARGS_8, TEMPLATE_ARG(9)
#define TEMPLATE_ARGS_10 TEMPLATE_ARGS_9, TEMPLATE_ARG(10)

#define ARGUMENT(n) const Arg##n &arg##n
#define ARGUMENTS_1  ARGUMENT(1)
#define ARGUMENTS_2  ARGUMENTS_1, ARGUMENT(2)
#define ARGUMENTS_3  ARGUMENTS_2, ARGUMENT(3)
#define ARGUMENTS_4  ARGUMENTS_3, ARGUMENT(4)
#define ARGUMENTS_5  ARGUMENTS_4, ARGUMENT(5)
#define ARGUMENTS_6  ARGUMENTS_5, ARGUMENT(6)
#define ARGUMENTS_7  ARGUMENTS_6, ARGUMENT(7)
#define ARGUMENTS_8  ARGUMENTS_7, ARGUMENT(8)
#define ARGUMENTS_9  ARGUMENTS_8, ARGUMENT(9)
#define ARGUMENTS_10 ARGUMENTS_9, ARGUMENT(10)

#define VALUE(n) arg##n
#define VALUES_1  VALUE(1)
#define VALUES_2  VALUES_1, VALUE(2)
#define VALUES_3  VALUES_2, VALUE(3)
#define VALUES_4  VALUES_3, VALUE(4)
#define VALUES_5  VALUES_4, VALUE(5)
#define VALUES_6  VALUES_5, VALUE(6)
#define VALUES_7  VALUES_6, VALUE(7)
#define VALUES_8  VALUES_7, VALUE(8)
#define VALUES_9  VALUES_8, VALUE(9)
#define VALUES_10 VALUES_9, VALUE(10)


class Allocator
{
public:
	typedef int size_type;

public:
	virtual ~Allocator() {}

	virtual void *allocate(size_type n, const void *hint = 0) = 0;
    virtual void deallocate(void *p, size_type n) = 0;
    virtual void deallocate(void *p) = 0;

    template <typename T>
    T *construct() { return new (allocate(sizeof(T))) T(); }

    template <typename T, TEMPLATE_ARGS_1>
    T *construct(ARGUMENTS_1) { return new (allocate(sizeof(T))) T(VALUES_1); }

    template <typename T, TEMPLATE_ARGS_2>
    T *construct(ARGUMENTS_2) { return new (allocate(sizeof(T))) T(VALUES_2); }

    template <typename T, TEMPLATE_ARGS_3>
    T *construct(ARGUMENTS_3) { return new (allocate(sizeof(T))) T(VALUES_3); }

    template <typename T, TEMPLATE_ARGS_4>
    T *construct(ARGUMENTS_4) { return new (allocate(sizeof(T))) T(VALUES_4); }

    template <typename T, TEMPLATE_ARGS_5>
    T *construct(ARGUMENTS_5) { return new (allocate(sizeof(T))) T(VALUES_5); }

    template <typename T, TEMPLATE_ARGS_6>
    T *construct(ARGUMENTS_6) { return new (allocate(sizeof(T))) T(VALUES_6); }

    template <typename T, TEMPLATE_ARGS_7>
    T *construct(ARGUMENTS_7) { return new (allocate(sizeof(T))) T(VALUES_7); }

    template <typename T, TEMPLATE_ARGS_8>
    T *construct(ARGUMENTS_8) { return new (allocate(sizeof(T))) T(VALUES_8); }

    template <typename T, TEMPLATE_ARGS_9>
    T *construct(ARGUMENTS_9) { return new (allocate(sizeof(T))) T(VALUES_9); }

    template <typename T, TEMPLATE_ARGS_10>
    T *construct(ARGUMENTS_10) { return new (allocate(sizeof(T))) T(VALUES_10); }

    template <typename T>
    void destroy(T *p) { deallocate(p); }
};


template <int C>
class ManagerBase
{
protected:
	Allocator **allocators() const { return m_allocators; }
	Allocator **allocators() { return m_allocators; }

private:
	Allocator *m_allocators[C];
};


template <typename T>
class ScopedPointer
{
public:
	typedef ScopedPointer<T>                               class_type;
	typedef typename Templates::remove_qualifiers<T>::type type;

public:
	ScopedPointer() :
		m_pointer(0),
		m_allocator(0)
	{}
	ScopedPointer(type *pointer, Allocator *allocator) :
		m_pointer(pointer),
		m_allocator(allocator)
	{}
	~ScopedPointer()
	{
		if (m_allocator)
			m_allocator->destroy(m_pointer);
	}

	const type &operator*() const { return *m_pointer; }
	type &operator*() { return *m_pointer; }

	const type *operator->() const { return m_pointer; }
	type *operator->() { return m_pointer; }

	type *take()
	{
		type *pointer = m_pointer;
		m_pointer = 0;
		return pointer;
	}

private:
	ScopedPointer(const class_type &other);
	void operator=(const class_type &other);

private:
	type *m_pointer;
	Allocator *m_allocator;
};


#undef TEMPLATE_ARG
#undef TEMPLATE_ARGS_1
#undef TEMPLATE_ARGS_2
#undef TEMPLATE_ARGS_3
#undef TEMPLATE_ARGS_4
#undef TEMPLATE_ARGS_5
#undef TEMPLATE_ARGS_6
#undef TEMPLATE_ARGS_7
#undef TEMPLATE_ARGS_8
#undef TEMPLATE_ARGS_9
#undef TEMPLATE_ARGS_10

#undef ARGUMENT
#undef ARGUMENTS_1
#undef ARGUMENTS_2
#undef ARGUMENTS_3
#undef ARGUMENTS_4
#undef ARGUMENTS_5
#undef ARGUMENTS_6
#undef ARGUMENTS_7
#undef ARGUMENTS_8
#undef ARGUMENTS_9
#undef ARGUMENTS_10

#undef VALUE
#undef VALUES_1
#undef VALUES_2
#undef VALUES_3
#undef VALUES_4
#undef VALUES_5
#undef VALUES_6
#undef VALUES_7
#undef VALUES_8
#undef VALUES_9
#undef VALUES_10

MEMORY_NS_END

#endif /* MEMORYMANAGERBASE_H_ */
