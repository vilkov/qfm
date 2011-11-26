#include "idmrootnode.h"
//#include "../items/idmroot.h"
//#include "../items/idmmessage.h"
//#include "../items/idmseparator.h"


IDM_PLUGIN_NS_BEGIN

IdmRootNode::IdmRootNode(const Info &storage, Node *parent) :
	IdmNode(IdmContainer(storage), storage.absolutePath(), parent)
{
//	if (m_container.isValid())
//		m_items.push_back(new IdmRoot(storage.absolutePath()));
//	else
//	{
//		m_items.push_back(new IdmSeparator());
//		m_items.push_back(new IdmMessage(m_container.lastError()));
//	}
}

IDM_PLUGIN_NS_END
