#ifndef GLOBAL_MEMORY_TAG_H_
#define GLOBAL_MEMORY_TAG_H_

#include <new>
#include "../memorymanager.h"

typedef std::size_t size_type;

inline void *operator new(size_type size) throw(std::bad_alloc)            { return MemoryManager::instance()->allocator(MemoryManager::Qt)->allocate(size); }
inline void *operator new[](size_type size) throw(std::bad_alloc)          { return MemoryManager::instance()->allocator(MemoryManager::Qt)->allocate(size); }
inline void *operator new(size_type size, const std::nothrow_t&) throw()   { return MemoryManager::instance()->allocator(MemoryManager::Qt)->allocate(size); }
inline void *operator new[](size_type size, const std::nothrow_t&) throw() { return MemoryManager::instance()->allocator(MemoryManager::Qt)->allocate(size); }
inline void operator delete(void *ptr, size_type size) throw()             { return MemoryManager::instance()->allocator(MemoryManager::Qt)->deallocate(ptr, size); }
inline void operator delete(void *ptr) throw()                             { return MemoryManager::instance()->allocator(MemoryManager::Qt)->deallocate(ptr); }
inline void operator delete[](void *ptr) throw()                           { return MemoryManager::instance()->allocator(MemoryManager::Qt)->deallocate(ptr); }
inline void operator delete(void *ptr, const std::nothrow_t&) throw()      { return MemoryManager::instance()->allocator(MemoryManager::Qt)->deallocate(ptr); }
inline void operator delete[](void *ptr, const std::nothrow_t&) throw()    { return MemoryManager::instance()->allocator(MemoryManager::Qt)->deallocate(ptr); }

#endif /* GLOBAL_MEMORY_TAG_H_ */
