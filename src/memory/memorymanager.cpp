#include "memorymanager.h"


static MemoryManager *instance = 0;


MemoryManager::MemoryManager()
{
	::instance = this;
}


MemoryManager *MemoryManager::instance()
{
	return ::instance;
}
