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
#ifndef MEMORYMANAGERBASE_H_
#define MEMORYMANAGERBASE_H_

#include "../memory_ns.h"
#include "../../templates/metatemplates.h"


MEMORY_NS_BEGIN

#include "../../arguments/define.h"

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
    T *construct(ARGUMENTS_BY_REFERENCE_1) { return new (allocate(sizeof(T))) T(VALUES_1); }

    template <typename T, TEMPLATE_ARGS_2>
    T *construct(ARGUMENTS_BY_REFERENCE_2) { return new (allocate(sizeof(T))) T(VALUES_2); }

    template <typename T, TEMPLATE_ARGS_3>
    T *construct(ARGUMENTS_BY_REFERENCE_3) { return new (allocate(sizeof(T))) T(VALUES_3); }

    template <typename T, TEMPLATE_ARGS_4>
    T *construct(ARGUMENTS_BY_REFERENCE_4) { return new (allocate(sizeof(T))) T(VALUES_4); }

    template <typename T, TEMPLATE_ARGS_5>
    T *construct(ARGUMENTS_BY_REFERENCE_5) { return new (allocate(sizeof(T))) T(VALUES_5); }

    template <typename T, TEMPLATE_ARGS_6>
    T *construct(ARGUMENTS_BY_REFERENCE_6) { return new (allocate(sizeof(T))) T(VALUES_6); }

    template <typename T, TEMPLATE_ARGS_7>
    T *construct(ARGUMENTS_BY_REFERENCE_7) { return new (allocate(sizeof(T))) T(VALUES_7); }

    template <typename T, TEMPLATE_ARGS_8>
    T *construct(ARGUMENTS_BY_REFERENCE_8) { return new (allocate(sizeof(T))) T(VALUES_8); }

    template <typename T, TEMPLATE_ARGS_9>
    T *construct(ARGUMENTS_BY_REFERENCE_9) { return new (allocate(sizeof(T))) T(VALUES_9); }

    template <typename T, TEMPLATE_ARGS_10>
    T *construct(ARGUMENTS_BY_REFERENCE_10) { return new (allocate(sizeof(T))) T(VALUES_10); }

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
	typedef ::Tools::Memory::ScopedPointer<T>                               class_type;
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

#include "../../arguments/undefine.h"

MEMORY_NS_END

#endif /* MEMORYMANAGERBASE_H_ */
