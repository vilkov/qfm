#ifndef IDMNODE_H_
#define IDMNODE_H_

#include "idmnodebase.h"


IDM_PLUGIN_NS_BEGIN

class IdmNode : public IdmNodeBase
{
public:
	IdmNode(const IdmContainer &container, const Info &info, Node *parent = 0);
};

IDM_PLUGIN_NS_END

#endif /* IDMNODE_H_ */
