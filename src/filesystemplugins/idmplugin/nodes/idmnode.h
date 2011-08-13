#ifndef IDMNODE_H_
#define IDMNODE_H_

#include "idmnodebase.h"


FILE_SYSTEM_NS_BEGIN

class IdmNode : public IdmNodeBase
{
public:
	IdmNode(IdmContainer *storage, const Info &info, Node *parent = 0);
};

FILE_SYSTEM_NS_END

#endif /* IDMNODE_H_ */
