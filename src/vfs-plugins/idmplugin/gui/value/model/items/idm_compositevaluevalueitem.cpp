#include "idm_compositevaluevalueitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueValueItem::CompositeValueValueItem(const EntityValue::Holder &value, IdmItem *parent) :
	CompositeValueItem(parent),
	m_value(value)
{}

QVariant CompositeValueValueItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

bool CompositeValueValueItem::isValue() const
{
	return true;
}

IDM_PLUGIN_NS_END
