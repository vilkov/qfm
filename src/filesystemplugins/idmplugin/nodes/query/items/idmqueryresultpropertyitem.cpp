#include "idmqueryresultpropertyitem.h"
#include "idmqueryresultvalueitem.h"
#include "idmqueryresultpathvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPropertyItem::QueryResultPropertyItem(const IdmEntity::Property &property, const IdmCompositeEntityValue::List &values, IdmItem *parent) :
	QueryResultListItem(parent),
	m_property(property)
{
	add(values);
}

QVariant QueryResultPropertyItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_property.name;
	else
		return QVariant();
}

bool QueryResultPropertyItem::isRoot()
{
	return false;
}

bool QueryResultPropertyItem::isProperty()
{
	return true;
}

bool QueryResultPropertyItem::isValue()
{
	return false;
}

void QueryResultPropertyItem::add(IdmEntityValue *value)
{
	if (value->entity()->type() == Database::Path)
		m_items.push_back(new QueryResultPathValueItem(value, this));
	else
		m_items.push_back(new QueryResultValueItem(value, this));
}

void QueryResultPropertyItem::add(const IdmCompositeEntityValue::List &values)
{
	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
		if (values.at(i)->entity()->type() == Database::Path)
			m_items.push_back(new QueryResultPathValueItem(values.at(i), this));
		else
			m_items.push_back(new QueryResultValueItem(values.at(i), this));
}

void QueryResultPropertyItem::remove(size_type index)
{
	delete m_items.takeAt(index);
}

IDM_PLUGIN_NS_END
