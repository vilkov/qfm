#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include "../tools/memory/memorymanagerbase.h"


class MemoryManager : public ::Tools::Memory::ManagerBase<3>
{
public:
	enum Allocators
	{
		Qt,
		Loki,
		Malloc
	};

public:
	MemoryManager();

	static MemoryManager *instance();
	::Tools::Memory::Allocator *allocator(Allocators a) { return allocators()[a]; }
};

#endif /* MEMORYMANAGER_H_ */
