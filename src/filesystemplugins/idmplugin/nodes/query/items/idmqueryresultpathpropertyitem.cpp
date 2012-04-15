#include "idmqueryresultpathpropertyitem.h"
#include "idmqueryresultpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"


IDM_PLUGIN_NS_BEGIN

inline static bool lessThan(const QueryResultItem *v1, const QueryResultItem *v2)
{
	return ProxyModel::compareByFileNames(static_cast<const QueryResultPathValueItem *>(v1), static_cast<const QueryResultPathValueItem *>(v2));
}


QueryResultPathPropertyItem::QueryResultPathPropertyItem(const IdmEntity::Property &property, Base *parent) :
	QueryResultPropertyItem(property, parent)
{}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const IdmEntityValue::Holder &value)
{
	m_items.push_back(new QueryResultPathValueItem(container, value, this));
	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const IdmCompositeEntityValue::List &values)
{
	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
		m_items.push_back(new QueryResultPathValueItem(container, values.at(i), this));

	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(TasksNode::TasksItemList &files, const IFileContainer *container, const IdmCompositeEntityValue::List &values)
{
	QueryResultItem *item;

	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
	{
		m_items.push_back(item = new QueryResultPathValueItem(container, values.at(i), this));
		files.push_back(item);
	}

	qSort(m_items.begin(), m_items.end(), lessThan);
}

IDM_PLUGIN_NS_END
