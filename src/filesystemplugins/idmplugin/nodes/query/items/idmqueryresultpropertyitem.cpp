#include "idmqueryresultpropertyitem.h"
#include "idmqueryresultvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPropertyItem::QueryResultPropertyItem(const IdmEntity::Property &property, Base *parent) :
	QueryResultItem(parent),
	m_property(property)
{}

QueryResultPropertyItem::Base *QueryResultPropertyItem::at(size_type index) const
{
	return m_items.at(index).data();
}

QueryResultPropertyItem::size_type QueryResultPropertyItem::size() const
{
	return m_items.size();
}

QueryResultPropertyItem::size_type QueryResultPropertyItem::indexOf(Base *item) const
{
	QueryResultItem::Holder holder(static_cast<QueryResultItem *>(item));
	return m_items.indexOf(holder);
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

bool QueryResultPropertyItem::isPath()
{
	return false;
}

bool QueryResultPropertyItem::isRootPathValue()
{
	return false;
}

void QueryResultPropertyItem::add(const IdmEntityValue::Holder &value)
{
	m_items.push_back(QueryResultItem::Holder(new QueryResultValueItem(value, this)));
}

void QueryResultPropertyItem::add(const IdmCompositeEntityValue::List &values)
{
	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
		m_items.push_back(QueryResultItem::Holder(new QueryResultValueItem(values.at(i), this)));
}

void QueryResultPropertyItem::remove(size_type index)
{
	m_items.removeAt(index);
}

IDM_PLUGIN_NS_END
