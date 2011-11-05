#include "idmqueryresultpropertyitem.h"
#include "idmqueryresultvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPropertyItem::QueryResultPropertyItem(const IdmEntity::Property &property, const IdmCompositeEntityValue::List &values, IdmItem *parent) :
	IdmListItem(parent),
	m_property(property)
{
	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
		m_items.push_back(new QueryResultValueItem(values.at(i), this));
}

QVariant QueryResultPropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_property.name;
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
