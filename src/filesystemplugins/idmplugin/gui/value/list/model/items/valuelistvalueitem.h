#ifndef VALUELISTVALUEITEM_H_
#define VALUELISTVALUEITEM_H_

#include "../../../../../items/idmentityvalueitem.h"


IDM_PLUGIN_NS_BEGIN

class ValueListValueItem : public IdmEntityValueItem
{
public:
	ValueListValueItem(IdmEntity *entity, const QVariant &value, IdmItem *parent = 0);

protected:
	friend class ValueListRootItem;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* VALUELISTVALUEITEM_H_ */
