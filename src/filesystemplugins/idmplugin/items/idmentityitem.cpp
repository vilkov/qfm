#include "idmentityitem.h"


FILE_SYSTEM_NS_BEGIN

IdmEntityItem::IdmEntityItem(IdmEntity *entity, IdmItem *parent) :
	IdmItemsList(parent),
	m_entity(entity)
{}

QVariant IdmEntityItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_entity->name();
	else
		return QVariant();
}

bool IdmEntityItem::isEntityItem() const
{
	return true;
}

FILE_SYSTEM_NS_END
