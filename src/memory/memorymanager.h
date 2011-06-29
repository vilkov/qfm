#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include "../tools/memory/memorymanagerbase.h"


class MemoryManager : public MemoryManagerBase<3>
{
public:
	enum Allocators
	{
		Malloc,
		Qt,
		Loki
	};

public:
	MemoryManager();

	static MemoryManager *instance();
	Allocator *allocator(Allocators a) { return allocators()[a]; }
};

#endif /* MEMORYMANAGER_H_ */
