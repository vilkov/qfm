#include "idm_compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePathItem::CompositeValuePathItem(const EntityValue::Holder &value, IdmItem *parent) :
	CompositeValueItem(parent),
	m_value(value)
{}

bool CompositeValuePathItem::isPath() const
{
	return true;
}

IDM_PLUGIN_NS_END
