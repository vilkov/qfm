#ifndef COMPOSITEVALUEITEM_H_
#define COMPOSITEVALUEITEM_H_

#include "../../../../model/items/idmitem.h"
#include "../../../../storage/values/idmentityvalue.h"


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

#endif /* COMPOSITEVALUEITEM_H_ */
