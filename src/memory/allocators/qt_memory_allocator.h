#ifndef QT_MEMORY_ALLOCATOR_H_
#define QT_MEMORY_ALLOCATOR_H_

#include <QtCore/QGlobalStatic>
#include "../../tools/memory/memorymanagerbase.h"


class QtAllocator : public ::Tools::Memory::Allocator
{
public:
	virtual void *allocate(size_type n, const void *hint = 0) { return qMalloc(n); }
    virtual void deallocate(void *p, size_type n) { qFree(p); }
    virtual void deallocate(void *p) { qFree(p); }
};

#endif /* QT_MEMORY_ALLOCATOR_H_ */
