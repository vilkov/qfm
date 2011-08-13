#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include "idmnodebase.h"


FILE_SYSTEM_NS_BEGIN

class IdmRootNode : public IdmNodeBase
{
public:
	IdmRootNode(const Info &storage, Node *parent = 0);

private:
	IdmContainer m_storage;
};

FILE_SYSTEM_NS_END

#endif /* IDMROOTNODE_H_ */
