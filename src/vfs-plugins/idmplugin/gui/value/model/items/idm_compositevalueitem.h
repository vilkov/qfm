#ifndef IDM_COMPOSITEVALUEITEM_H_
#define IDM_COMPOSITEVALUEITEM_H_

#include "../../../../model/items/idm_item.h"
#include "../../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueItem : public IdmItem
{
public:
	CompositeValueItem(IdmItem *parent = 0);

	virtual bool isPath() const;
	virtual bool isValue() const;
	virtual bool isProperty() const;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEITEM_H_ */
