#ifndef COMPOSITEVALUEITEM_H_
#define COMPOSITEVALUEITEM_H_

#include "../../../../model/items/idmlistitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueItem : public IdmListItem
{
public:
	CompositeValueItem(IdmItem *parent = 0);
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEITEM_H_ */
