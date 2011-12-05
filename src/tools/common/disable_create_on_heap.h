#ifndef DISABLE_CREATE_ON_HEAP_H_
#define DISABLE_CREATE_ON_HEAP_H_

#include <new>

#define DISABLE_CREATE_ON_HEAP                                                     \
	void* operator new(std::size_t) throw (std::bad_alloc) { return 0; }           \
	void* operator new[](std::size_t) throw (std::bad_alloc) { return 0; }         \
	void operator delete(void*) throw() {}                                         \
	void operator delete[](void*) throw() {}                                       \
	void* operator new(std::size_t, const std::nothrow_t&) throw() { return 0; }   \
	void* operator new[](std::size_t, const std::nothrow_t&) throw() { return 0; } \
	void operator delete(void*, const std::nothrow_t&) throw() {}                  \
	void operator delete[](void*, const std::nothrow_t&) throw() {}                \
                                                                                   \
	inline void* operator new(std::size_t, void* __p) throw() { return __p; }      \
	inline void* operator new[](std::size_t, void* __p) throw() { return __p; }    \
                                                                                   \
	inline void  operator delete  (void*, void*) throw() {}                        \
	inline void  operator delete[](void*, void*) throw() {}                        \


#endif /* DISABLE_CREATE_ON_HEAP_H_ */
