#include "application.h"
#include "memory/memorymanager.h"


int main(int argc, char *argv[])
{
	MemoryManagerBase<1> manager;

//	MemoryManager manager(4096, 64, 4);
	return Application(QString::fromLatin1("qfm"), QString::fromLatin1("Qt based file manager"), QString(), argc, argv, true).exec();
}
