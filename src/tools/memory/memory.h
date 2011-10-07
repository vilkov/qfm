#ifndef MEMORY_H_
#define MEMORY_H_

#include <new>


void *operator new[](size_t size) throw (std::bad_alloc);
void *operator new(size_t size) throw (std::bad_alloc);
void operator delete[](void *ptr) throw ();
void operator delete[](void *ptr, size_t) throw ();
void operator delete(void *ptr) throw ();
void operator delete(void *ptr, size_t) throw ();

#endif /* MEMORY_H_ */
