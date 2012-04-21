#ifndef MALLOC_MEMORY_TAG_H_
#define MALLOC_MEMORY_TAG_H_

#include <new>
#include "../memorymanager.h"


class malloc_memory_manager_tag
{
public:
	typedef Memory::Allocator::size_type size_type;

public:
	inline void *operator new(size_type size) throw(std::bad_alloc)            { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->allocate(size); }
	inline void *operator new[](size_type size) throw(std::bad_alloc)          { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->allocate(size); }
	inline void *operator new(size_type size, const std::nothrow_t&) throw()   { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->allocate(size); }
	inline void *operator new[](size_type size, const std::nothrow_t&) throw() { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->allocate(size); }
	inline void operator delete(void *ptr, size_type size) throw()             { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->deallocate(ptr, size); }
	inline void operator delete(void *ptr) throw()                             { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->deallocate(ptr); }
	inline void operator delete[](void *ptr) throw()                           { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->deallocate(ptr); }
	inline void operator delete(void *ptr, const std::nothrow_t&) throw()      { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->deallocate(ptr); }
	inline void operator delete[](void *ptr, const std::nothrow_t&) throw()    { return MemoryManager::instance()->allocator(MemoryManager::Malloc)->deallocate(ptr); }

	// Default placement versions of operator new.
	inline void *operator new(size_type, void *ptr) throw() { return ptr; }
	inline void *operator new[](size_type, void *ptr) throw() { return ptr; }

	// Default placement versions of operator delete.
	inline void operator delete(void*, void*) throw() {}
	inline void operator delete[](void*, void*) throw() {}
};

#endif /* MALLOC_MEMORY_TAG_H_ */
