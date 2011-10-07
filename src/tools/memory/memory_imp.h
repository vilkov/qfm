#ifndef MEMORY_IMP_H_
#define MEMORY_IMP_H_

#include <QMap>
#include <QMutex>
#include <stdlib.h>
#include "memory.h"


namespace Memory
{
	static QMutex mutex;
	static size_t leaks = 0;
	static QMap<size_t, size_t> statistics;
}

void *operator new(size_t size) throw (std::bad_alloc)
{
	QMutexLocker locker(&Memory::mutex);
	++Memory::statistics[size];
	Memory::leaks += size;
    return malloc(size);
}

void *operator new[](size_t size) throw (std::bad_alloc)
{
	QMutexLocker locker(&Memory::mutex);
	++Memory::statistics[size];
	Memory::leaks += size;
    return malloc(size);
}

void operator delete(void *ptr) throw ()
{
	QMutexLocker locker(&Memory::mutex);
    free(ptr);
}

void operator delete(void *ptr, size_t size) throw ()
{
	QMutexLocker locker(&Memory::mutex);
	Memory::leaks -= size;
    free(ptr);
}

void operator delete[](void *ptr) throw ()
{
	QMutexLocker locker(&Memory::mutex);
    free(ptr);
}

void operator delete[](void *ptr, size_t size) throw ()
{
	QMutexLocker locker(&Memory::mutex);
	Memory::leaks -= size;
    free(ptr);
}

#endif /* MEMORY_IMP_H_ */
