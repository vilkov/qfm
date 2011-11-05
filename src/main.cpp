#include "application.h"
#include "memory/memorymanager.h"


int main(int argc, char *argv[])
{
//	MemoryManager manager;
	return Application(QString::fromLatin1("qfm"), QString::fromLatin1("Qt based file manager"), QString(), argc, argv, true).exec();
}
