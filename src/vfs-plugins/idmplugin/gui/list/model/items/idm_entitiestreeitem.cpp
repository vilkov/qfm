#include "idm_entitiestreeitem.h"


IDM_PLUGIN_NS_BEGIN

IdmEntitiesTreeItem::IdmEntitiesTreeItem(IdmEntity *entity, IdmItem *parent) :
	IdmListItem(parent),
	m_entity(entity)
{}

QVariant IdmEntitiesTreeItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_entity->name();
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
