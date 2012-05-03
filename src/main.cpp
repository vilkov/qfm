#include "application.h"
//#include "memory/memorymanager.h"


int main(int argc, char *argv[])
{
//	MemoryManager manager;
	return Application(
			QString::fromLatin1("qfm"),
			QString::fromLatin1("qde"),
			QString::fromLatin1("Qt based file manager"),
			argc, argv, true).exec();
}
