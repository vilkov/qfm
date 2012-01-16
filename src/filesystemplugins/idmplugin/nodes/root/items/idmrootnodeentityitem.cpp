#include "idmrootnodeentityitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodeEntityItem::RootNodeEntityItem(IdmEntity *entity, Base *parent) :
	RootNodeListItem(parent),
	m_entity(entity)
{}

QVariant RootNodeEntityItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_entity->name();
	else
		return QVariant();
}

bool RootNodeEntityItem::isEntity()
{
	return true;
}

IDM_PLUGIN_NS_END
