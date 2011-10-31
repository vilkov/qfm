#ifndef VALUESTREEVALUEITEM_H_
#define VALUESTREEVALUEITEM_H_

#include "../../../../../../items/idmentityvalueitem.h"


IDM_PLUGIN_NS_BEGIN

class ValuesTreeValueItem : public IdmEntityValueItem
{
public:
	ValuesTreeValueItem(IdmEntityValue *value, IdmItem *parent = 0);

	const IdmEntityValue *value() const { return m_value; }

protected:
	friend class CompositeValueModel;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* VALUESTREEVALUEITEM_H_ */
