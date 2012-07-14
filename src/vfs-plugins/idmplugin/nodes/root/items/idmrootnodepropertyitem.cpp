#include "idmrootnodepropertyitem.h"


IDM_PLUGIN_NS_BEGIN

RootNodePropertyItem::RootNodePropertyItem(const IdmEntity::Property &property, Base *parent) :
	RootNodeEntityItem(property.entity, parent),
	m_property(property),
	m_label(QString(m_property.name).append(QString::fromLatin1(" (")).append(m_property.entity->name()).append(QChar(L')')))
{}

RootNodePropertyItem::RootNodePropertyItem(IdmEntity *property, const QString &name, Base *parent) :
	RootNodeEntityItem(property, parent),
	m_property(property, name),
	m_label(QString(m_property.name).append(QString::fromLatin1(" (")).append(m_property.entity->name()).append(QChar(L')')))
{}

QVariant RootNodePropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

bool RootNodePropertyItem::isProperty()
{
	return true;
}

IDM_PLUGIN_NS_END
