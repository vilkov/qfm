#include "compositevaluevalueitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueValueItem::CompositeValueValueItem(IdmEntityValue *value, IdmItem *parent) :
	CompositeValueItem(parent),
	m_value(value)
{}

CompositeValueValueItem::~CompositeValueValueItem()
{
	delete m_value;
}

QVariant CompositeValueValueItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
