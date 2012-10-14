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
#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <new>
//#include <QMap>
#include <QMutex>
#include "SmallObj.h"


//void *operator new(std::size_t) throw(std::bad_alloc);
//void *operator new[](std::size_t) throw(std::bad_alloc);
//void *operator new(std::size_t, const std::nothrow_t&) throw();
//void *operator new[](std::size_t, const std::nothrow_t&) throw();
//void operator delete(void*, std::size_t) throw();
//void operator delete(void*) throw();
//void operator delete[](void*) throw();
//void operator delete(void*, const std::nothrow_t&) throw();
//void operator delete[](void*, const std::nothrow_t&) throw();


class MemoryManager
{
public:
	MemoryManager(std::size_t pageSize, std::size_t maxObjectSize, std::size_t objectAlignSize);
	~MemoryManager();

	bool isValid() const { return !m_allocator.isCorrupt(); }

	void *allocate(std::size_t size);
    void deallocate(void *p, std::size_t size);
    void deallocate(void *p);

    static MemoryManager *instance();

private:
	QMutex m_mutex;
//	qint64 m_leaks;
//	QMap<qint64, qint64> m_statistics;
	Loki::SmallObjAllocator m_allocator;
};


class MemoryManagerTag
{
public:
//	inline void *operator new(std::size_t size) throw(std::bad_alloc)             { return MemoryManager::instance()->allocate(size); }
//	inline void *operator new[](std::size_t size) throw(std::bad_alloc)           { return MemoryManager::instance()->allocate(size); }
//	inline void *operator new(std::size_t size, const std::nothrow_t&) throw()    { return MemoryManager::instance()->allocate(size); }
//	inline void *operator new[](std::size_t size, const std::nothrow_t&) throw()  { return MemoryManager::instance()->allocate(size); }
//	inline void operator delete(void *ptr, std::size_t size) throw()              { return MemoryManager::instance()->deallocate(ptr, size); }
//	inline void operator delete(void *ptr) throw()                                { return MemoryManager::instance()->deallocate(ptr); }
//	inline void operator delete[](void *ptr) throw()                              { return MemoryManager::instance()->deallocate(ptr); }
//	inline void operator delete(void *ptr, const std::nothrow_t&) throw()         { return MemoryManager::instance()->deallocate(ptr); }
//	inline void operator delete[](void *ptr, const std::nothrow_t&) throw()       { return MemoryManager::instance()->deallocate(ptr); }
//
//	// Default placement versions of operator new.
//	inline void *operator new(std::size_t, void *ptr) throw() { return ptr; }
//	inline void *operator new[](std::size_t, void *ptr) throw() { return ptr; }
//
//	// Default placement versions of operator delete.
//	inline void operator delete(void*, void*) throw() {}
//	inline void operator delete[](void*, void*) throw() {}
};

#endif /* MEMORY_MANAGER_H_ */

//#if defined(LOKI_SMALL_OBJECT_USE_NEW_ARRAY) && defined(_MSC_VER)
//#pragma message("Don't define LOKI_SMALL_OBJECT_USE_NEW_ARRAY when using a Microsoft compiler to prevent memory leaks.")
//#pragma message("now calling '#undef LOKI_SMALL_OBJECT_USE_NEW_ARRAY'")
//#undef LOKI_SMALL_OBJECT_USE_NEW_ARRAY
//#endif
