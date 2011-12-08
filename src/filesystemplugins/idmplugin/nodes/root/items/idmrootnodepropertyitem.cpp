#include "idmrootnodepropertyitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodePropertyItem::RootNodePropertyItem(const IdmEntity::Property &property, Base *parent) :
	RootNodeEntityBaseListItem(parent),
	m_property(property)
{}

QVariant RootNodePropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_property.name;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
