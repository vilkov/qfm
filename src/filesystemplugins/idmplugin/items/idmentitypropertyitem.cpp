#include "idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

IdmEntityPropertyItem::IdmEntityPropertyItem(const IdmEntity::Property &property, IdmItem *parent) :
	IdmEntityItem(property.entity, parent),
	m_property(property)
{}

QVariant IdmEntityPropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		switch (column)
		{
			case 0: return m_property.name;
			case 1: return m_entity->name();
		}

	return QVariant();
}

IDM_PLUGIN_NS_END
