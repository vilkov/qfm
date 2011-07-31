#ifndef IDMBASENODE_H_
#define IDMBASENODE_H_

#include "../../../filesystem/filesystemnode.h"
#include "../../../filesystem/filesystempluginsmanager.h"


FILE_SYSTEM_NS_BEGIN

class IdmBaseNode : public Node
{
public:
	IdmBaseNode(Node *parent = 0) :
		Node(parent)
	{}
};

FILE_SYSTEM_NS_END

#endif /* IDMBASENODE_H_ */
