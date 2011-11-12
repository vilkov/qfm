#include "compositevaluevalueitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueValueItem::CompositeValueValueItem(IdmEntityValue *value, bool owner, IdmItem *parent) :
	IdmItem(parent),
	m_value(value),
	m_owner(owner)
{}

CompositeValueValueItem::~CompositeValueValueItem()
{
	if (m_owner)
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
