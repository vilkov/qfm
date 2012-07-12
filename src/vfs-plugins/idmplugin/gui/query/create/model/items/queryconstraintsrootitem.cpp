#include "queryconstraintsrootitem.h"


IDM_PLUGIN_NS_BEGIN

QueryConstraintsRootItem::QueryConstraintsRootItem() :
	GroupConstraint(And)
{}

void QueryConstraintsRootItem::clear()
{
	m_items.clear();
}

IDM_PLUGIN_NS_END
