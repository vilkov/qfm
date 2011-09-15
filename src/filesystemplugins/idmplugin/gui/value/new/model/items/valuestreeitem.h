#ifndef VALUESTREEITEM_H_
#define VALUESTREEITEM_H_

#include "../../../../../items/idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class ValuesTreeItem : public IdmEntityPropertyItem
{
public:
	ValuesTreeItem(IdmEntity *entity, const QString &name, IdmItem *parent = 0);

protected:
	friend class ValuesRootTreeItem;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* VALUESTREEITEM_H_ */
