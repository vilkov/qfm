#ifndef MALLOC_MEMORY_ALLOCATOR_H_
#define MALLOC_MEMORY_ALLOCATOR_H_

#include <stdlib.h>
#include "../../tools/memory/memorymanagerbase.h"


class MallocAllocator : public Memory::Allocator
{
public:
	virtual void *allocate(size_type n, const void *hint = 0) { return malloc(n); }
    virtual void deallocate(void *p, size_type n) { free(p); }
    virtual void deallocate(void *p) { free(p); }
};

#endif /* MALLOC_MEMORY_ALLOCATOR_H_ */
