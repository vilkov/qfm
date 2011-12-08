#include "idmrootnodeentityitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodeEntityItem::RootNodeEntityItem(IdmEntity *entity, Base *parent) :
	RootNodeEntityBaseListItem(parent),
	m_entity(entity)
{}

QVariant RootNodeEntityItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_entity->name();
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
