#include "idmnode.h"
//#include "../items/idmroot.h"
//#include "../items/idmmessage.h"
//#include "../items/idmseparator.h"


IDM_PLUGIN_NS_BEGIN

IdmNode::IdmNode(const IdmContainer &container, const Info &info, Node *parent) :
	IdmNodeBase(container, info, parent)
{
//	if (container.isValid())
//	{
////		m_items.push_back(storage->menu());
//		m_items.push_back(new IdmRoot(info));
//	}
//	else
//	{
//		m_items.push_back(new IdmSeparator());
//		m_items.push_back(new IdmMessage(container.lastError()));
//	}
}

IDM_PLUGIN_NS_END
