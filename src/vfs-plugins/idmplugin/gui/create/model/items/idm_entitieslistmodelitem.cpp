#include "idm_entitieslistmodelitem.h"


IDM_PLUGIN_NS_BEGIN

IdmEntitiesListItem::IdmEntitiesListItem(const Entity::Property &property, IdmItem *parent) :
	IdmListItem(parent),
	m_property(property)
{}

QVariant IdmEntitiesListItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		switch (column)
		{
			case 0: return m_property.name;
			case 1: return m_property.entity->name();
		}

	return QVariant();
}

IDM_PLUGIN_NS_END
