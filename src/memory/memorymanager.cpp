#include "memorymanager.h"
#include "allocators/qt_memory_allocator.h"
#include "allocators/loki_memory_allocator.h"
#include "allocators/malloc_memory_allocator.h"


static MemoryManager *instance = 0;
static QtAllocator qtAllocator;
static LokiAllocator lokiAllocator(4096, 64, 4);
static MallocAllocator mallocAllocator;


MemoryManager::MemoryManager()
{
	::instance = this;
	allocators()[Qt] = &qtAllocator;
	allocators()[Loki] = &lokiAllocator;
	allocators()[Malloc] = &mallocAllocator;
}

MemoryManager *MemoryManager::instance()
{
	return ::instance;
}
