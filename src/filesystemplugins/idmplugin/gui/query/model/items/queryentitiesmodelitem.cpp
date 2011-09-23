#include "queryentitiesmodelitem.h"


IDM_PLUGIN_NS_BEGIN

QueryEntitiesModelItem::QueryEntitiesModelItem(const QString &name, IdmEntity *entity, IdmItem *parent) :
	IdmEntityItem(entity, parent),
	m_name(name)
{}

QVariant QueryEntitiesModelItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_name;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
