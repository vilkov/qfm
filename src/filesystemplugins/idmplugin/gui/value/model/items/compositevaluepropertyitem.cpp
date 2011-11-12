#include "compositevaluepropertyitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePropertyItem::CompositeValuePropertyItem(const IdmEntity::Property &property, IdmItem *parent) :
	IdmListItem(parent),
	m_property(property)
{}

QVariant CompositeValuePropertyItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_property.name;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
