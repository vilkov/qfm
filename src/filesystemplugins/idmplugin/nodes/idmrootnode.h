#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include "idmnode.h"


IDM_PLUGIN_NS_BEGIN

class IdmRootNode : public IdmNode
{
public:
	IdmRootNode(const Info &storage, Node *parent = 0);
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODE_H_ */
