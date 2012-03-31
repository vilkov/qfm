#include "compositevaluepropertyitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePropertyItem::CompositeValuePropertyItem(const IdmEntity::Property &property, IdmItem *parent) :
	CompositeValueItem(parent),
	m_property(property)
{}

CompositeValuePropertyItem::Base *CompositeValuePropertyItem::at(size_type index) const
{
	return m_items.at(index);
}

CompositeValuePropertyItem::size_type CompositeValuePropertyItem::size() const
{
	return m_items.size();
}

CompositeValuePropertyItem::size_type CompositeValuePropertyItem::indexOf(Base *item) const
{
	return m_items.indexOf(item);
}

QVariant CompositeValuePropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_property.name;
	else
		return QVariant();
}

bool CompositeValuePropertyItem::isProperty() const
{
	return true;
}

IDM_PLUGIN_NS_END
