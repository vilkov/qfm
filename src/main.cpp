#include "application.h"
#include "tools/memory/memory_manager.h"
#include "tools/avltree/avltree.h"


class AvlTest : public AvlTree<int, int>
{
protected:
	virtual AvlNode* create_node(const key_type &value, AvlNode *&parent) { return new AvlNode(value, parent); }

};



extern "C" int main(int argc, char *argv[])
{
	MemoryManager manager(4096, 64, 4);
	return Application(QString::fromLatin1("qfm"), QString::fromLatin1("Qt based file manager"), QString(), argc, argv, true).exec();
}
