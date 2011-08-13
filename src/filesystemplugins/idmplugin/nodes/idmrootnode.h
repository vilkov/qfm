#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include "idmnode.h"


FILE_SYSTEM_NS_BEGIN

class IdmRootNode : public IdmNode
{
public:
	IdmRootNode(const QFileInfo &storage, Node *parent = 0);

private:
	IdmStorage m_storage;
};

FILE_SYSTEM_NS_END

#endif /* IDMROOTNODE_H_ */
