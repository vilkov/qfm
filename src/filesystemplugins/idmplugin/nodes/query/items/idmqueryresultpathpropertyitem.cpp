#include "idmqueryresultpathpropertyitem.h"
#include "idmqueryresultrootpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"


IDM_PLUGIN_NS_BEGIN

inline static bool lessThan(const QueryResultItem::Holder &v1, const QueryResultItem::Holder &v2)
{
	return ProxyModel::compareByFileNames(v1.as<QueryResultRootPathValueItem>(), v2.as<QueryResultRootPathValueItem>());
}


QueryResultPathPropertyItem::QueryResultPathPropertyItem(const IdmEntity::Property &property, Base *parent) :
	QueryResultPropertyItem(property, parent)
{}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const IdmEntityValue::Holder &value)
{
	m_items.push_back(Holder(new QueryResultRootPathValueItem(container, value, this)));
	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const IdmCompositeEntityValue::List &values)
{
	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
		m_items.push_back(Holder(new QueryResultRootPathValueItem(container, values.at(i), this)));

	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(Snapshot::Files &files, const IFileContainer *container, const IdmCompositeEntityValue::List &values)
{
	Holder item;

	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
	{
		m_items.push_back(item = new QueryResultRootPathValueItem(container, values.at(i), this));
		files.add(item.as<QueryResultRootPathValueItem>()->fileName(), item);
	}

	qSort(m_items.begin(), m_items.end(), lessThan);
}

IDM_PLUGIN_NS_END
