#include "idm_compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueItem::CompositeValueItem(IdmItem *parent) :
	IdmItem(parent)
{}

bool CompositeValueItem::isPath() const
{
	return false;
}

bool CompositeValueItem::isValue() const
{
	return false;
}

bool CompositeValueItem::isProperty() const
{
	return false;
}

IDM_PLUGIN_NS_END
