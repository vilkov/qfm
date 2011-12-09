#include "idmrootnodepropertyitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodePropertyItem::RootNodePropertyItem(const IdmEntity::Property &property, Base *parent) :
	RootNodeEntityItem(property.entity, parent),
	m_property(property),
	m_label(QString(m_property.name).append(QString::fromLatin1(" (")).append(m_property.entity->name()).append(QChar(')')))
{}

QVariant RootNodePropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
