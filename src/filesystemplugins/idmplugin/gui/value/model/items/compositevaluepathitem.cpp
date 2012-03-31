#include "compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePathItem::CompositeValuePathItem(const IdmEntityValue::Holder &value, IdmItem *parent) :
	CompositeValueItem(parent),
	m_value(value)
{}

QVariant CompositeValuePathItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

bool CompositeValuePathItem::isPath() const
{
	return true;
}

IDM_PLUGIN_NS_END
