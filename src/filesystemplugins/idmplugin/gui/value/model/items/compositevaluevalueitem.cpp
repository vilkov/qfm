#include "compositevaluevalueitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueValueItem::CompositeValueValueItem(IdmEntityValue *value, IdmItem *parent) :
	IdmItem(parent),
	m_value(value)
{}

QVariant CompositeValueValueItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}


CompositeValueValueOwnerItem::CompositeValueValueOwnerItem(IdmEntityValue *value, IdmItem *parent) :
	CompositeValueValueItem(value, parent)
{}

CompositeValueValueOwnerItem::~CompositeValueValueOwnerItem()
{
	delete m_value;
}

IDM_PLUGIN_NS_END
