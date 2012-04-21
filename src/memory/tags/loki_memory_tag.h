#ifndef LOKI_MEMORY_TAG_H_
#define LOKI_MEMORY_TAG_H_

#include <new>
#include "../memorymanager.h"


class loki_memory_manager_tag
{
public:
	typedef Memory::Allocator::size_type size_type;

public:
	inline void *operator new(size_type size) throw(std::bad_alloc)            { return MemoryManager::instance()->allocator(MemoryManager::Loki)->allocate(size); }
	inline void *operator new[](size_type size) throw(std::bad_alloc)          { return MemoryManager::instance()->allocator(MemoryManager::Loki)->allocate(size); }
	inline void *operator new(size_type size, const std::nothrow_t&) throw()   { return MemoryManager::instance()->allocator(MemoryManager::Loki)->allocate(size); }
	inline void *operator new[](size_type size, const std::nothrow_t&) throw() { return MemoryManager::instance()->allocator(MemoryManager::Loki)->allocate(size); }
	inline void operator delete(void *ptr, size_type size) throw()             { return MemoryManager::instance()->allocator(MemoryManager::Loki)->deallocate(ptr, size); }
	inline void operator delete(void *ptr) throw()                             { return MemoryManager::instance()->allocator(MemoryManager::Loki)->deallocate(ptr); }
	inline void operator delete[](void *ptr) throw()                           { return MemoryManager::instance()->allocator(MemoryManager::Loki)->deallocate(ptr); }
	inline void operator delete(void *ptr, const std::nothrow_t&) throw()      { return MemoryManager::instance()->allocator(MemoryManager::Loki)->deallocate(ptr); }
	inline void operator delete[](void *ptr, const std::nothrow_t&) throw()    { return MemoryManager::instance()->allocator(MemoryManager::Loki)->deallocate(ptr); }

	// Default placement versions of operator new.
	inline void *operator new(size_type, void *ptr) throw() { return ptr; }
	inline void *operator new[](size_type, void *ptr) throw() { return ptr; }

	// Default placement versions of operator delete.
	inline void operator delete(void*, void*) throw() {}
	inline void operator delete[](void*, void*) throw() {}
};

#endif /* LOKI_MEMORY_TAG_H_ */
