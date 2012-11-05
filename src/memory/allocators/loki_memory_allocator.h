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
#ifndef LOKI_MEMORY_ALLOCATOR_H_
#define LOKI_MEMORY_ALLOCATOR_H_

#include <tools/memory/memorymanagerbase.h>
#include "SmallObj.h"


class LokiAllocator : public ::Tools::Memory::Allocator
{
public:
	LokiAllocator(size_type pageSize, size_type maxObjectSize, size_type objectAlignSize) :
		m_allocator(pageSize, maxObjectSize, objectAlignSize)
	{}

	bool isValid() const { return !m_allocator.isCorrupt(); }

	virtual void *allocate(size_type n, const void *hint = 0) { return m_allocator.allocate(n); }
    virtual void deallocate(void *p, size_type n) { m_allocator.deallocate(p, n); }
    virtual void deallocate(void *p) { m_allocator.deallocate(p); }

private:
	::Loki::SmallObjAllocator m_allocator;
};

#endif /* LOKI_MEMORY_ALLOCATOR_H_ */
