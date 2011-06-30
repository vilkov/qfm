#ifndef LOKI_MEMORY_ALLOCATOR_H_
#define LOKI_MEMORY_ALLOCATOR_H_

#include "SmallObj.h"
#include "../../tools/memory/memorymanagerbase.h"


class LokiAllocator : public Memory::Allocator
{
public:
	LokiAllocator(size_type pageSize, size_type maxObjectSize, size_type objectAlignSize) :
		m_allocator(pageSize, maxObjectSize, objectAlignSize)
	{}

	bool isValid() const { return !m_allocator.isCorrupt(); }

	virtual void *allocate(size_type n, const void *hint = 0) { m_allocator.allocate(n); }
    virtual void deallocate(void *p, size_type n) { m_allocator.deallocate(p, n); }
    virtual void deallocate(void *p) { m_allocator.deallocate(p); }

private:
	Loki::SmallObjAllocator m_allocator;
};

#endif /* LOKI_MEMORY_ALLOCATOR_H_ */
