#include "queryentitiesmodelitem.h"


IDM_PLUGIN_NS_BEGIN

QueryEntitiesModelItem::QueryEntitiesModelItem(const IdmEntity::Property &property, IdmItem *parent) :
	IdmListItem(parent),
	m_property(property)
{}

QVariant QueryEntitiesModelItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_property.name;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
