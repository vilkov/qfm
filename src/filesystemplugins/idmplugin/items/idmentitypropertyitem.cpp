#include "idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

IdmEntityPropertyItem::IdmEntityPropertyItem(const QString &name, IdmEntity *entity, IdmItem *parent) :
	IdmEntityItem(entity, parent),
	m_name(name)
{}

QVariant IdmEntityPropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		switch (column)
		{
			case 0: return m_name;
			case 1: return m_entity->name();
		}

	return QVariant();
}

IDM_PLUGIN_NS_END
