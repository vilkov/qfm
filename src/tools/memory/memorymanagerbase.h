#ifndef MEMORYMANAGERBASE_H_
#define MEMORYMANAGERBASE_H_

#include "../metatemplates.h"


template <int C>
class MemoryManagerBase
{
public:
	class Allocator
	{
	public:
		typedef qint32 size_type;

	public:
		virtual ~Allocator() {}

		virtual void *allocate(size_type n, const void *hint = 0) = 0;
	    virtual void deallocate(void *p, size_type n) = 0;
	    virtual void deallocate(void *p) = 0;

	    template <typename T>
	    T *construct() { return new (allocate(sizeof(T))) T(); }

	    template <typename T, typename Arg1>
	    T *construct(const Arg1 &arg1) { return new (allocate(sizeof(T))) T(arg1); }

	    template <typename T>
	    void destroy(T *p) { deallocate(p); }
	};

public:
	MemoryManagerBase()
	{
		allocators()[0] = 0;
	}

protected:
	Allocator **allocators() const { return m_allocators; }
	Allocator **allocators() { return m_allocators; }

private:
	Allocator *m_allocators[C];
};

#endif /* MEMORYMANAGERBASE_H_ */
