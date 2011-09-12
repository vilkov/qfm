#ifndef VALUESTREEITEM_H_
#define VALUESTREEITEM_H_

#include "../../../../items/idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

class ValuesTreeItem : public IdmEntityItem
{
public:
	ValuesTreeItem(IdmEntity *entity, IdmItem *parent = 0);

protected:
	friend class ValuesRootTreeItem;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* VALUESTREEITEM_H_ */
