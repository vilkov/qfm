#include "idmrootnode.h"
#include "../items/idmroot.h"
#include "../items/idmmessage.h"
#include "../items/idmseparator.h"


IDM_PLUGIN_NS_BEGIN

IdmRootNode::IdmRootNode(const Info &storage, Node *parent) :
	IdmNodeBase(&m_storage, storage.absolutePath(), parent),
	m_storage(storage)
{
	if (m_storage.isValid())
	{
//		m_items.push_back(m_storage.menu());
		m_items.push_back(new IdmRoot(storage.absolutePath()));
	}
	else
	{
		m_items.push_back(new IdmSeparator());
		m_items.push_back(new IdmMessage(m_storage.lastError()));
	}
}

IDM_PLUGIN_NS_END
