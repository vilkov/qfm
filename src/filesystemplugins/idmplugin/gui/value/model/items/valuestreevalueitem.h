#ifndef VALUESTREEVALUEITEM_H_
#define VALUESTREEVALUEITEM_H_

#include "../../../../items/idmentityvalueitem.h"


IDM_PLUGIN_NS_BEGIN

class ValuesTreeValueItem : public IdmEntityValueItem
{
public:
	ValuesTreeValueItem(IdmEntity *entity, const QVariant &value, IdmItem *parent = 0);

protected:
	friend class ValuesRootTreeItem;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* VALUESTREEVALUEITEM_H_ */
