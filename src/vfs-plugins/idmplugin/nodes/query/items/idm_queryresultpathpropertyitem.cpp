#include "idm_queryresultpathpropertyitem.h"
#include "idm_queryresultrootpathvalueitem.h"

#include <vfs/model/vfs_proxymodel.h>


IDM_PLUGIN_NS_BEGIN

inline static bool lessThan(const QueryResultItem::Holder &v1, const QueryResultItem::Holder &v2)
{
	return ProxyModel::compareByFileNames(v1.as<QueryResultRootPathValueItem>()->info(), v2.as<QueryResultRootPathValueItem>()->info());
}


QueryResultPathPropertyItem::QueryResultPathPropertyItem(const Entity::Property &property, Base *parent) :
	QueryResultPropertyItem(property, parent)
{}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const EntityValue::Holder &value)
{
	m_items.push_back(Holder(new QueryResultRootPathValueItem(container, value, this)));
	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const CompositeEntityValue::List &values)
{
	for (CompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
		m_items.push_back(Holder(new QueryResultRootPathValueItem(container, values.at(i), this)));

	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(Snapshot::Files &files, const IFileContainer *container, const CompositeEntityValue::List &values)
{
	Holder item;

	for (CompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
	{
		m_items.push_back(item = new QueryResultRootPathValueItem(container, values.at(i), this));
		files.add(item.as<QueryResultRootPathValueItem>()->info()->fileName(), item);
	}

	qSort(m_items.begin(), m_items.end(), lessThan);
}

IDM_PLUGIN_NS_END
