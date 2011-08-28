#ifndef IDMMENUENTITIESTREEITEM_H_
#define IDMMENUENTITIESTREEITEM_H_

#include "../../../../items/idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesTreeItem : public IdmEntityItem
{
public:
	IdmEntitiesTreeItem(IdmEntity *entity, IdmItem *parent = 0);

protected:
	friend class IdmEntitiesTreeItemRoot;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* IDMMENUENTITIESTREEITEM_H_ */
