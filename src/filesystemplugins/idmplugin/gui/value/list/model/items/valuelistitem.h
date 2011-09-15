#ifndef VALUELISTITEM_H_
#define VALUELISTITEM_H_

#include "../../../../../items/idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class ValueListItem : public IdmEntityPropertyItem
{
public:
	ValueListItem(IdmEntity *entity, const QString &name, IdmItem *parent = 0);

protected:
	friend class ValueListRootItem;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* VALUELISTITEM_H_ */
