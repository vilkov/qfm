#include "idmnode.h"
#include "../items/idmroot.h"
#include "../items/idmmessage.h"
#include "../items/idmseparator.h"


IDM_PLUGIN_NS_BEGIN

IdmNode::IdmNode(IdmContainer *storage, const Info &info, Node *parent) :
	IdmNodeBase(storage, info, parent)
{
	if (storage->isValid())
	{
		m_items.push_back(storage->menu());
		m_items.push_back(new IdmRoot(info));
	}
	else
	{
		m_items.push_back(new IdmSeparator());
		m_items.push_back(new IdmMessage(storage->lastError()));
	}
}

IDM_PLUGIN_NS_END
